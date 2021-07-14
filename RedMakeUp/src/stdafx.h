#pragma once

#ifdef  RMU_PLATFORM_WINDOWS
	// Windows stuff
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
	// ComPtr
	#include <wrl.h>
	// DirectX 12
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	#include <d3dx12.h>// Helper header
#endif //  RMU_PLATFORM_WINDOWS

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <optional>

#include "RMU/Logger.h"
#include "RMU/Common.h"
