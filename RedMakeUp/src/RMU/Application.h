#pragma once

#include "Window.h"
#include "Graphics.h"
#include "Timer.h"

namespace RMU {
	class RMU_API Application {
	public:
		Application();
		virtual ~Application();

		bool Init();

		int Run();

		/// <summary>
		/// Called when a frame is coming
		/// </summary>
		void DoFrame(float deltaTime);

	private:
		void CalcFrameRate(float deltaTime);

	private:
		std::unique_ptr<Window> m_mainWindow;
		std::unique_ptr<Graphics> m_graphics;
		Timer m_timer;
	};

	/// <summary>
	/// Must be implemented in client.
	/// </summary>
	/// <returns>A pointer to an instance of Application or its derived class</returns>
	std::unique_ptr<Application> CreateApp();
}
