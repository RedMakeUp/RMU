#pragma once

#ifdef RMU_PLATFORM_WINDOWS
	#define RMU_API
#else
	#error RedMakeUp only support Windows
#endif

#ifdef  RMU_PLATFORM_WINDOWS
	// Windows stuff
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
	// ComPtr
	#include <wrl.h>
	using Microsoft::WRL::ComPtr;
	// DirectX 12
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	#include <d3dx12.h>// Helper header
#endif //  RMU_PLATFORM_WINDOWS

/// <summary>
/// Check calling result of d3d12 api and throw an exception if failed
/// </summary>
/// <param name="hr">return value of d3d12 api calling</param>
inline void ThrowIfFailed(HRESULT hr) {
#ifdef RMU_DEBUG
	if (FAILED(hr)) {
		throw std::exception();
	}
#endif // RMU_DEBUG
}