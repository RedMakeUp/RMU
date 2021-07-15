#include "stdafx.h"
#include "Graphics.h"

namespace RMU {
	void Graphics::Init()
	{
		EnableDebugLayer();

		m_device = CreateDevice(GetAdapter());
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
}
