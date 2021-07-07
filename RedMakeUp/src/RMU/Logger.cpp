#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

// Initialize static variable
std::shared_ptr<spdlog::logger> RMU::Logger::s_logger;


void RMU::Logger::Init() noexcept
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_logger = spdlog::stdout_color_mt("RMU");
	s_logger->set_level(spdlog::level::trace);
}
