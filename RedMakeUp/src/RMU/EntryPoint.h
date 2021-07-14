#pragma once

#include <memory>
#include "Logger.h"

#ifdef RMU_PLATFORM_WINDOWS
extern std::unique_ptr<RMU::Application> RMU::CreateApp();

int main(int argc, char** argv) {
	// Force only one instance of the app is running
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, L"MyApp1.0");
	if (!hMutex) hMutex = CreateMutex(0, 0, L"MyApp1.0");
	else {
		MessageBoxA(NULL, "RedMakeUp has been run", "RedMakeUp", MB_OK);
		return 0;
	}

	RMU::Logger::Init();
	RMU_LOG_INFO("Initialize Done!");

	auto app = RMU::CreateApp();
	app->Init();
	int code = app->Run();

	return code;
}
#endif