#pragma once

#include "Log/Log.hpp"
#include <memory>

namespace drop 
{

	class IPlatform
	{
	public:

		virtual ~IPlatform() = default;

		IPlatform(const IPlatform&) = delete;
		IPlatform& operator=(const IPlatform&) = delete;
		IPlatform(IPlatform&&) = delete;
		IPlatform& operator=(IPlatform&&) = delete;
		
		static IPlatform* Create(void* hInstance);

		static void Initialize();
		static void Terminate() { IPlatform::Instance()->TerminateImpl(); }

		static IPlatform* Instance() noexcept { return s_Instance; }

		static void Destroy() { delete s_Instance; s_Instance = nullptr; }

		template<typename T>
		static T* As() noexcept
		{
			static_assert(std::is_base_of<IPlatform, T>::value, "T must inherit from IPlatform");
			return static_cast<T*>(s_Instance);
		}

	protected:
		IPlatform() = default;

	private:

		virtual void InitializeImpl() = 0;
		virtual void TerminateImpl() = 0;

	private:
		static IPlatform* s_Instance;

	};

}