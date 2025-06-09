#pragma once

namespace DP {

	struct ApplicationConfig
	{

	};

	class Application
	{
	public:

		Application(const ApplicationConfig& config = {}) {}
		virtual ~Application() {}

		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) = delete;

		void OnUpdate() {}

		void Print();


	private:

	};
}