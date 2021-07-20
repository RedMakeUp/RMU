#include "stdafx.h"
#include "Graphics.h"

namespace RMU {
	void Graphics::Init(HWND hWnd)
	{
		EnableDebugLayer();

		m_device = CreateDevice(GetAdapter());

		m_rtvSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// TODO: Create swap chain and descriptor heap below this
	}

	void Graphics::EnableDebugLayer() const
	{
#ifdef RMU_DEBUG
		ComPtr<ID3D12Debug> d;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&d)));
		d->EnableDebugLayer();
		RMU_LOG_INFO("D3D debug layer is on");
#else
		RMU_LOG_INFO("D3D debug layer is off");
#endif // RMU_DEBUG
	}

	ComPtr<IDXGIAdapter4> Graphics::GetAdapter(bool useWarp) const
	{
		ComPtr<IDXGIFactory4> factory;

		UINT flag = 0;
#ifdef RMU_DEBUG
		flag = DXGI_CREATE_FACTORY_DEBUG;
#endif // RMU_DEBUG
		ThrowIfFailed(CreateDXGIFactory2(flag, IID_PPV_ARGS(&factory)));

		ComPtr<IDXGIAdapter1> adapter1;
		ComPtr<IDXGIAdapter4> adapter4;

		if (useWarp) {// WARP adapter
			ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter1)));
			ThrowIfFailed(adapter1.As(&adapter4));
			RMU_LOG_INFO("Software adapter was selected");
		}
		else {// Hardware adapter
			// Pick the one with biggest dedicated video memory
			size_t maxDedicatedVideoMemory = 0;
			for (UINT i = 0; factory->EnumAdapters1(i, &adapter1) != DXGI_ERROR_NOT_FOUND; ++i) {
				DXGI_ADAPTER_DESC1 desc1;
				adapter1->GetDesc1(&desc1);
				// Check if it is a hareware adapter
				if (!(desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)) {
					// Check d3d12 support
					if (SUCCEEDED(D3D12CreateDevice(adapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
						desc1.DedicatedVideoMemory > maxDedicatedVideoMemory) 
					{
						maxDedicatedVideoMemory = desc1.DedicatedVideoMemory;
						ThrowIfFailed(adapter1.As(&adapter4));
					}
				}
			}

			DXGI_ADAPTER_DESC1 desc1;
			adapter4->GetDesc1(&desc1);
			constexpr float factor = 1.0f / 1024 / 1024 / 1024;
			RMU_LOG_INFO("Hardware adapter was selected: video memory - {} GB", desc1.DedicatedVideoMemory * factor);
		}

		return adapter4;
	}
	
	ComPtr<ID3D12Device2> Graphics::CreateDevice(ComPtr<IDXGIAdapter4> adapter) const
	{
		if (!adapter) throw std::exception("Empty adapter");

		ComPtr<ID3D12Device2> device2;
		ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device2)));

		// Enable debug messages in debug mode.
		// https://www.3dgep.com/learning-directx-12-1/#Preamble
#ifdef RMU_DEBUG
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		if (SUCCEEDED(device2.As(&pInfoQueue)))
		{
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

			// Suppress whole categories of messages
			//D3D12_MESSAGE_CATEGORY Categories[] = {};

			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY Severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO
			};

			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID DenyIds[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
			};

			D3D12_INFO_QUEUE_FILTER NewFilter = {};
			//NewFilter.DenyList.NumCategories = _countof(Categories);
			//NewFilter.DenyList.pCategoryList = Categories;
			NewFilter.DenyList.NumSeverities = _countof(Severities);
			NewFilter.DenyList.pSeverityList = Severities;
			NewFilter.DenyList.NumIDs = _countof(DenyIds);
			NewFilter.DenyList.pIDList = DenyIds;

			ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
		}
#endif
		return device2;
	}

	ComPtr<IDXGISwapChain4> Graphics::CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, UINT width, UINT height, size_t bufferCount) const
	{
		ComPtr<IDXGISwapChain4> swapChain;

		ComPtr<IDXGIFactory4> factory;
		UINT flag = 0;
#ifdef RMU_DEBUG
		flag = DXGI_CREATE_FACTORY_DEBUG;
#endif // RMU_DEBUG
		ThrowIfFailed(CreateDXGIFactory2(flag, IID_PPV_ARGS(&factory)));

		DXGI_SWAP_CHAIN_DESC1 desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Stereo = FALSE;
		desc.SampleDesc = { 1, 0 };
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = BUFFER_NUM;
		desc.Scaling = DXGI_SCALING_STRETCH;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		desc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ComPtr<IDXGISwapChain1> swapChain1;
		ThrowIfFailed(factory->CreateSwapChainForHwnd(
			commandQueue.Get(),
			hWnd,
			&desc,
			nullptr,
			nullptr,
			&swapChain1
		));

		ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
		
		ThrowIfFailed(swapChain1.As(&swapChain));

		return swapChain;
	}

	ComPtr<ID3D12CommandQueue> Graphics::CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) const
	{
		ComPtr<ID3D12CommandQueue> queue;

		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = type;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		ThrowIfFailed(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue)));

		return queue;
	}

	ComPtr<ID3D12DescriptorHeap> Graphics::CreateDescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors) const
	{
		ComPtr<ID3D12DescriptorHeap> heap;

		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = numDescriptors;
		desc.Type = type;

		ThrowIfFailed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap)));

		return heap;
	}

	ComPtr<ID3D12CommandAllocator> Graphics::CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) const
	{
		ComPtr<ID3D12CommandAllocator> allocator;

		ThrowIfFailed(device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator)));

		return allocator;
	}

	ComPtr<ID3D12GraphicsCommandList> Graphics::CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type) const
	{
		ComPtr<ID3D12GraphicsCommandList> list;

		ThrowIfFailed(device->CreateCommandList(0, type, allocator.Get(), nullptr, IID_PPV_ARGS(&list)));
		ThrowIfFailed(list->Close());

		return list;
	}

	ComPtr<ID3D12Fence> Graphics::CreateFence(ComPtr<ID3D12Device2> device) const
	{
		ComPtr<ID3D12Fence> fence;

		ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

		return fence;
	}

	HANDLE Graphics::CreateEventHandle() const
	{
		HANDLE fenceEvent;

		fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (!fenceEvent) {
			RMU_LOG_ERROR("Failed to create fence event handle");
			throw std::exception("Empty Fence Event");
		}

		return fenceEvent;
	}

	void Graphics::Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue)
	{
		ThrowIfFailed(commandQueue->Signal(fence.Get(), ++fenceValue));
	}

	void Graphics::WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration) const
	{
		if (fence->GetCompletedValue() < fenceValue) {
			ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
			::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
		}
	}

	void Graphics::Flush(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue, HANDLE fenceEvent)
	{
		Signal(commandQueue, fence, fenceValue);
		WaitForFenceValue(fence, fenceValue, fenceEvent);
	}

	void Graphics::UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> heap)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(heap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < BUFFER_NUM; ++i) {
			ComPtr<ID3D12Resource> backBuffer;
			ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
			device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
			m_backBuffers[i] = backBuffer;
			rtvHandle.Offset(m_rtvSize);
		}
	}

	bool Graphics::CheckTearingSupport() const
	{
		BOOL allowTearing = FALSE;

		ComPtr<IDXGIFactory4> factory4;
		if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4)))) {
			ComPtr<IDXGIFactory5> factory5;
			if (SUCCEEDED(factory4.As(&factory5))) {
				if (FAILED(factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing)))) {
					allowTearing = FALSE;
				}
			}
		}

		return allowTearing == TRUE;
	}
}
