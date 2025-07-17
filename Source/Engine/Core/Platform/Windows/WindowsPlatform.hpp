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
		void PumpMessagesImpl() override;

		void SetHighDPIAwarenessEnabled(bool enabled);
		WCHAR* CreateWideString(const char* str) const;
		std::unique_ptr<WCHAR[]> CreateScopedWideString(const char* str) const;

		HINSTANCE GetHInstance() const { return m_HInstance; }
		int GetDPI() const { return m_DPI; }

		virtual void* AllocateImpl(size_t size) override;
		virtual void FreeImpl(void* ptr) override;
		virtual void ZeroMemoryImpl(void* ptr, size_t size) override;
		virtual void CopyMemoryImpl(void* dest, const void* src, size_t size) override;
		virtual void SetMemoryImpl(void* ptr, int value, size_t size) override;

	private:
		HINSTANCE m_HInstance;
		int m_DPI;
	};

}