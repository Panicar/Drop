#include <iostream>

#include <Application/Application.hpp>
#include <fmt/format.h>

#include <spdlog/spdlog.h>

int main()
{
	std::cout << "Engine is running..." << '\n';

	DP::Application app;
	app.Print();

	spdlog::info("gotveren ble naxuy");


#if DP_DEV_MODE

	std::cout << "Dev mode" << '\n';

#elif DP_PROFILE_MODE

	std::cout << "Profile Mode" << '\n';

#elif DP_MASTER_MODE
	
	std::cout << "Master Mode" << '\n';

#endif 

	while (true);

	return 0;
}