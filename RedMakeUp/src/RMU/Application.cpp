#include "stdafx.h"
#include "Application.h"

RMU::Application::Application()
{

}

RMU::Application::~Application()
{
}

bool RMU::Application::Init()
{
	if(!Window::Register()){
		return false;
	}
	m_mainWindow = std::make_unique<Window>(1280, 768, L"RedMakeUp Engine");

	m_graphics = std::make_unique<Graphics>();
	m_graphics->Init(m_mainWindow->GetHwnd());

	m_timer.Reset();

	return true;
}

int RMU::Application::Run()
{
	while (true)
	{
		// Process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// If return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		m_timer.Tick();

		DoFrame(m_timer.GetDeltaTime());
	}
}

void RMU::Application::DoFrame(float deltaTime)
{
	CalcFrameRate(deltaTime);
}

void RMU::Application::CalcFrameRate(float deltaTime)
{
	static size_t frameCounter = 0;
	static float elapsedSeconds = 0.0f;

	frameCounter++;
	elapsedSeconds += deltaTime;
	if (elapsedSeconds > 1.0f) {
		auto fps = static_cast<int>(frameCounter / elapsedSeconds);
		RMU_LOG_TRACE("FPS: {}", fps);

		frameCounter = 0;
		elapsedSeconds -= 1.0f;
	}
}
