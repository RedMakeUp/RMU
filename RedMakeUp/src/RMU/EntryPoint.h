#pragma once

#include <memory>
#include "Logger.h"

#ifdef RMU_PLATFORM_WINDOWS
extern std::unique_ptr<RMU::Application> RMU::CreateApp();

int main(int argc, char** argv) {
	RMU::Logger::Init();
	RMU_LOG_TRACE("Initialize Done!");

	auto app = RMU::CreateApp();
	int code = app->Run();

	return code;
}
#endif