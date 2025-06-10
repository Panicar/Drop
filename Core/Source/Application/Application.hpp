#pragma once

namespace DP {

	class CoreApplication
	{
	public:

		CoreApplication();
		virtual ~CoreApplication();

		CoreApplication(const CoreApplication& other) = delete;
		CoreApplication(CoreApplication&& other) = delete;
		CoreApplication& operator=(const CoreApplication& other) = delete;
		CoreApplication& operator=(CoreApplication&& other) = delete;

		void OnUpdate() {}
		
		void Execute();

		// Testing
		void Print();
		
		static CoreApplication* Instance() { return s_Instance; }

	private:
		static CoreApplication* s_Instance;
	};
}