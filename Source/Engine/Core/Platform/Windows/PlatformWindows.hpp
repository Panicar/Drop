#pragma once

#include "Platform/Platform.hpp"

#include "Core.hpp"


namespace drop {

	class PlatformWindows : public IPlatform
	{
	public:

		PlatformWindows(void* hInstance);
		~PlatformWindows() override;
		void InitializeImpl() override;
		void TerminateImpl() override;
			
	private:
		HINSTANCE m_HInstance;
	};

}