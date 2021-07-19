#pragma once

namespace RMU {
	class Graphics {
	public:
		/// <summary>
		/// Number of back buffers
		/// </summary>
		static const size_t BUFFER_NUM = 3;
		
		/// <summary>
		/// Whether to use software rasterizer or not
		/// </summary>
		static const bool USE_WARP = false;

	public:
		/// <summary>
		/// Initialize the graphics
		/// </summary>
		void Init(HWND hWnd);

		/// <summary>
		/// Returns whether all of d3d objects have been created
		/// </summary>
		inline bool IsInit() const { return m_isInit; }

	private:
		/// <summary>
		/// Enabling debug layer before anything about DXGI and D3D12 can help in identifying incorrect usage of APIs.
		/// </summary>
		void EnableDebugLayer() const;

		/// <summary>
		/// Get a d3d12 compatible adapter
		/// </summary>
		/// <param name="useWarp">Whether use software rasterizer or not</param>
		ComPtr<IDXGIAdapter4> GetAdapter(bool useWarp = USE_WARP) const;

		/// <summary>
		/// Create a d3d12 device which can be considered as a memory context that tracks allocations in GPU memory
		/// </summary>
		/// <param name="adapter">The adapter to create d3d12 device</param>
		ComPtr<ID3D12Device2> CreateDevice(ComPtr<IDXGIAdapter4> adapter) const;

		/// <summary>
		/// Create a DXGI swap chain
		/// </summary>
		ComPtr<IDXGISwapChain4> CreateSwapChain(HWND hWnd, ComPtr<ID3D12CommandQueue> commandQueue, size_t width, size_t height, size_t bufferCount) const;

		/// <summary>
		/// Create a command queue for a specific type
		/// </summary>
		ComPtr<ID3D12CommandQueue> CreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) const;

		/// <summary>
		/// Create a descriptor heap with specific type and the number of descripotrs
		/// </summary>
		ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptors) const;

		/// <summary>
		/// Create a command allocator which is actual memory of commands for specific type
		/// </summary>
		ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type) const;

		/// <summary>
		/// Create a command list using a command allocator as its backing memory
		/// </summary>
		ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12Device2> device, ComPtr<ID3D12CommandAllocator> allocator, D3D12_COMMAND_LIST_TYPE type) const;

		/// <summary>
		/// Create a CPU/GPU synchronization fence whih 0 as initial value
		/// </summary>
		ComPtr<ID3D12Fence> CreateFence(ComPtr<ID3D12Device2> device) const;

		/// <summary>
		/// Create a OS event handle to block the CPU thread until the fence has been signaled
		/// </summary>
		HANDLE CreateEventHandle() const;

		/// <summary>
		/// Signal the fence from GPU.
		/// fenceValue will be increased by one, and the new value is the target value for which we wait it from GPU
		/// </summary>
		void Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence, uint64_t& fenceValue);

		/// <summary>
		/// Block the thread until the inner value in fence is not less than fenceValue, and duration is the time limit
		/// </summary>
		void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent, std::chrono::milliseconds duration = std::chrono::milliseconds::max()) const;

		/// <summary>
		/// Get back buffers from the swap chain and create the Render Target View for them in descriptor heap
		/// </summary>
		void UpdateRenderTargetViews(ComPtr<ID3D12Device2> device, ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> heap);

		/// <summary>
		/// Check tearing is supported or not
		/// </summary>
		bool CheckTearingSupport() const;

	private:
		/// <summary>
		/// Whether all of d3d objects have been created
		/// </summary>
		bool m_isInit = false;

		/// <summary>
		/// Whether Vertical-Sync is on
		/// </summary>
		bool m_isVSync = true;

		/// <summary>
		/// Full screen mode or window mode
		/// </summary>
		bool m_isFullScreen = false;

		/// <summary>
		/// Index of current back buffer
		/// </summary>
		size_t m_currentBackBuffer = 0;

		/// <summary>
		/// Next value to signal the fence from command queue
		/// </summary>
		uint64_t m_fenceValue = 0;

		/// <summary>
		/// Keep trace of the "fence value" for specific back buffer which is used at certain frame
		/// </summary>
		uint64_t m_frameFenceValues[BUFFER_NUM] = {};

		/// <summary>
		/// The size of a render target view
		/// </summary>
		UINT m_rtvSize = 0;

		/// <summary>
		/// Receive the notification that the fence has reached a specific value
		/// </summary>
		HANDLE m_fenceEvent;

		/// <summary>
		/// D3D12 device
		/// </summary>
		ComPtr<ID3D12Device2> m_device;

		/// <summary>
		/// D3D12 command queue
		/// </summary>
		ComPtr<ID3D12CommandQueue> m_commandQueue;

		/// <summary>
		/// D3D12 command allocators which are actual memory to store commands for each back buffer
		/// </summary>
		ComPtr<ID3D12CommandAllocator> m_commandAllocators[BUFFER_NUM];

		/// <summary>
		/// D3D12 graphics command list to record gpu commands
		/// </summary>
		ComPtr<ID3D12GraphicsCommandList> m_commandList;

		/// <summary>
		/// Swap chain
		/// </summary>
		ComPtr<IDXGISwapChain4> m_swapChain;

		/// <summary>
		/// Back buffers
		/// </summary>
		ComPtr<ID3D12Resource> m_backBuffers[BUFFER_NUM];

		/// <summary>
		/// Descriptor heap for render target views
		/// </summary>
		ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;

		/// <summary>
		/// D3D12 fence to perform cpu-gpu sync
		/// </summary>
		ComPtr<ID3D12Fence> m_fence;
	};
}