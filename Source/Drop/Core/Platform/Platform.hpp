#pragma once

#include <memory>

namespace drop {

	class IPlatform
	{
	public:

		virtual ~IPlatform() = default;

		/*IPlatform(const IPlatform&) = delete;
		IPlatform& operator=(const IPlatform&) = delete;
		IPlatform(IPlatform&&) = delete;
		IPlatform& operator=(IPlatform&&) = delete;*/

		static void Initialize(void* instance = nullptr);
		static void Terminate() { IPlatform::Instance()->TerminateImpl(); }

		static IPlatform* Instance() noexcept { return s_Instance.get(); }

	private:
		static std::unique_ptr<IPlatform> Create();


		virtual void InitializeImpl(void* instance) = 0;
		virtual void TerminateImpl() = 0;

	private:
		static std::unique_ptr<IPlatform> s_Instance;

	};

}