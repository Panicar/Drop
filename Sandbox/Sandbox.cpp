#include "Sandbox.hpp"

#include <iostream>
#include <fmt/format.h>
#include <spdlog/spdlog.h>


Sandbox::Sandbox()
{
	std::cout << "Sandbox created" << '\n';

	SandboxTest();
}

void Sandbox::SandboxTest()
{

#if DP_DEV_MODE

	std::cout << "Dev mode" << '\n';

#elif DP_PROFILE_MODE

	std::cout << "Profile Mode" << '\n';

#elif DP_MASTER_MODE

	std::cout << "Master Mode" << '\n';

#endif 



}

DP_IMPLEMENT_APP(Sandbox);