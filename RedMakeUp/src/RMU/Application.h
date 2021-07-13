#pragma once

namespace RMU {
	class Window;

	class RMU_API Application {
	public:
		Application();
		virtual ~Application();

		int Run();

		void DoFrame();

	private:
		std::unique_ptr<Window> m_mainWindow;
	};

	/// <summary>
	/// Must be implemented in client.
	/// </summary>
	/// <returns>A pointer to an instance of Application or its derived class</returns>
	std::unique_ptr<Application> CreateApp();
}
