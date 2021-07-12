#pragma once

namespace RMU {
	class RMU_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	/// <summary>
	/// Must be implemented in client.
	/// </summary>
	/// <returns>A pointer to an instance of Application or its derived class</returns>
	std::unique_ptr<Application> CreateApp();
}
