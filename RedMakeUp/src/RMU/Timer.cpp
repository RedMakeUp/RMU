
#include "stdafx.h"
#include "Timer.h"

using namespace std::chrono;

RMU::Timer::Timer() noexcept
	:m_last(high_resolution_clock::now()), m_deltaTime(0.0f)
{

}

void RMU::Timer::Tick() noexcept
{
	auto current = high_resolution_clock::now();
	m_deltaTime = duration_cast<duration<float>>(current - m_last).count();
	m_last = current;

	if (m_deltaTime < 0.0f) m_deltaTime = 0.0f;
}

void RMU::Timer::Reset() noexcept
{
	m_deltaTime = 0.0f;
	m_last = high_resolution_clock::now();
}
