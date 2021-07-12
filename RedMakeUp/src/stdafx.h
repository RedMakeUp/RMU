#pragma once

#ifdef  RMU_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif //  RMU_PLATFORM_WINDOWS

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "RMU/Logger.h"
#include "RMU/Common.h"
