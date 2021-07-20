#pragma once
#include <chrono>

namespace RMU {
	class Timer {
	public:
		Timer() noexcept;

		Timer(const Timer&) = delete;
		Timer& operator=(const Timer&) = delete;

		void Tick() noexcept;
		void Reset() noexcept;

		inline float GetDeltaTime() const noexcept { return m_deltaTime; }
	private:
		std::chrono::high_resolution_clock::time_point m_last;
		float m_deltaTime;
	};
}