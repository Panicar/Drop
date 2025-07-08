#pragma once

#include "Core.hpp"
#include "Platform/Base/Platform.hpp"

namespace drop 
{

	class WindowsPlatform : public IPlatform
	{
	public:

		WindowsPlatform(void* hInstance);
		~WindowsPlatform() override;
		void InitializeImpl() override;
		void TerminateImpl() override;
			
		HINSTANCE GetHInstance() const { return m_HInstance; }

	private:
		HINSTANCE m_HInstance;
	};

}