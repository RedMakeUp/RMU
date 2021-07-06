#include <RedMakeUp.h>

class SandBox : public RMU::Application {
public:
	SandBox() = default;
	~SandBox() override = default;
};

std::unique_ptr<RMU::Application> RMU::CreateApp() {
	return std::make_unique<SandBox>();
}