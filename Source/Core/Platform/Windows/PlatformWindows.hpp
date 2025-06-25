#pragma once

#include "Platform/Platform.hpp"

#include "Core.hpp"

namespace drop {

	class PlatformWindows : public IPlatform
	{
	public:

		void InitializeImpl(void* instance) override;
		void TerminateImpl() override;
			
	private:

	};

}