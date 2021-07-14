#pragma once

// -- Windows Stuffs ----------------------------------------------- //
#ifdef  RMU_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
#endif //  RMU_PLATFORM_WINDOWS

// -- STL Stuffs --------------------------------------------------- //
#include <memory>

// -- Engine Core -------------------------------------------------- //
#include "RMU/Common.h"
#include "RMU/Application.h"
#include "RMU/Logger.h"
#include "RMU/Window.h"

// -- Entry Point -------------------------------------------------- //
#include "RMU/EntryPoint.h"
