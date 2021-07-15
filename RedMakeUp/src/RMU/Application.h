#pragma once

#include "Window.h"
#include "Graphics.h"

namespace RMU {
	class RMU_API Application {
	public:
		Application();
		virtual ~Application();

		bool Init();

		int Run();

		void DoFrame();

	private:
		std::unique_ptr<Window> m_mainWindow;
		std::unique_ptr<Graphics> m_graphics;
	};

	/// <summary>
	/// Must be implemented in client.
	/// </summary>
	/// <returns>A pointer to an instance of Application or its derived class</returns>
	std::unique_ptr<Application> CreateApp();
}
