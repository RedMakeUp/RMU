#pragma once

#include "Common.h"
#include <memory>

namespace RMU {
	class  Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// Must be implemented in client
	std::unique_ptr<Application> CreateApp();
}
