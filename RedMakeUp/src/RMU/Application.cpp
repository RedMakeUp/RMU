#include "stdafx.h"
#include "Application.h"
#include "Window.h"

RMU::Application::Application()
{
	m_mainWindow = std::make_unique<Window>(1280, 768, L"RedMakeUp Engine");

}

RMU::Application::~Application()
{
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
		DoFrame();
	}
}

void RMU::Application::DoFrame()
{
}
