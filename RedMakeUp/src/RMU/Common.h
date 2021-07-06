#pragma once

#ifdef RMU_PLATFORM_WINDOWS
	#define RMU_API
#else
	#error RedMakeUp only support Windows
#endif