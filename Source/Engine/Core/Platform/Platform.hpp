#pragma once

#include "Log/Log.hpp"

#include <memory>

namespace drop {

	class IPlatform
	{
	public:

		virtual ~IPlatform() { DP_CORE_DEBUG("IPlatform is destroyed!"); }

		/*IPlatform(const IPlatform&) = delete;
		IPlatform& operator=(const IPlatform&) = delete;
		IPlatform(IPlatform&&) = delete;
		IPlatform& operator=(IPlatform&&) = delete;*/
		
		static IPlatform* Create(void* hInstance);

		static void Initialize();
		static void Terminate() { IPlatform::Instance()->TerminateImpl(); }

		static IPlatform* Instance() noexcept { return s_Instance; }

	private:

		virtual void InitializeImpl() = 0;
		virtual void TerminateImpl() = 0;

	private:
		static IPlatform* s_Instance;

	};

}