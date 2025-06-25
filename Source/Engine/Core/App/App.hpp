#pragma once

#include "Core.hpp"

namespace drop {

	class CoreApp
	{
	public:

		CoreApp();
		virtual ~CoreApp();

		CoreApp(const CoreApp& other) = delete;
		CoreApp(CoreApp&& other) = delete;
		CoreApp& operator=(const CoreApp& other) = delete;
		CoreApp& operator=(CoreApp&& other) = delete;

		void OnUpdate() {}
		
		void Execute();

		// Testing
		void Print();
		
		static CoreApp* Instance() { return s_Instance; }

	private:
		static CoreApp* s_Instance;

		bool m_IsRunning = true;
		
	};

}