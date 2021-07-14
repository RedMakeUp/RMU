#pragma once

#ifdef RMU_PLATFORM_WINDOWS
	#define RMU_API
#else
	#error RedMakeUp only support Windows
#endif

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