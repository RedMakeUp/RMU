#pragma once

#include "Common.h"
#include <memory>
#include <spdlog/spdlog.h>

namespace RMU {

	class RMU_API Logger {
	public:
		/// <summary>
		/// Initialize the logger.
		/// Note: This function should be called at the very beginning of the program.
		/// </summary>
		static void Init() noexcept;

		/// <summary>
		/// Get the underlying logger.
		/// </summary>
		/// <returns>The underlying logger</returns>
		static const std::shared_ptr<spdlog::logger>& GetLogger() noexcept { return s_logger; }
	private:
		/// <summary>
		/// We use spdlog as the underlying logger.
		/// </summary>
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#define RMU_LOG_TRACE(...) ::RMU::Logger::GetLogger()->trace(__VA_ARGS__)
#define RMU_LOG_INFO(...) ::RMU::Logger::GetLogger()->info(__VA_ARGS__)
#define RMU_LOG_WARN(...) ::RMU::Logger::GetLogger()->warn(__VA_ARGS__)
#define RMU_LOG_ERROR(...) ::RMU::Logger::GetLogger()->error(__VA_ARGS__)
#define RMU_LOG_CRITICAL(...) ::RMU::Logger::GetLogger()->critical(__VA_ARGS__)