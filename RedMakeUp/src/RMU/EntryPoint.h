#pragma once

#include <memory>

#ifdef RMU_PLATFORM_WINDOWS
extern std::unique_ptr<RMU::Application> RMU::CreateApp();

int main(int argc, char** argv) {
	auto app = RMU::CreateApp();
	app->Run();

	return 0;
}
#endif