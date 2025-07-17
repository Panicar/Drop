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
		static void Terminate() { s_Instance->TerminateImpl(); }
		static void PumpMessages() { s_Instance->PumpMessagesImpl(); }
		static IPlatform* Instance() noexcept { return s_Instance; }
		static void Destroy() { delete s_Instance; s_Instance = nullptr; }
		


		template<typename T>
		static T* As() noexcept
		{
			static_assert(std::is_base_of<IPlatform, T>::value, "T must inherit from IPlatform");
			return static_cast<T*>(s_Instance);
		}

		static void* Allocate(size_t size) { s_Instance->AllocateImpl(size); }
		//static void* AllocateAligned(size_t size, size_t alignment) { return s_Instance->AllocateAlignedImpl(size, alignment); }
		static void Free(void* ptr) { s_Instance->FreeImpl(ptr); }
		static void ZeroMemory(void* ptr, size_t size) { s_Instance->ZeroMemoryImpl(ptr, size); }
		static void CopyMemory(void* dest, const void* src, size_t size) { s_Instance->CopyMemoryImpl(dest, src, size); }
		static void SetMemory(void* ptr, int value, size_t size) { s_Instance->SetMemoryImpl(ptr, value, size); }

	protected:
		IPlatform() = default;

	private:

		virtual void InitializeImpl() = 0;
		virtual void TerminateImpl() = 0;
		virtual void PumpMessagesImpl() = 0;

		virtual void* AllocateImpl(size_t size) = 0;
		//virtual void* AllocateAlignedImpl(size_t size, size_t alignment) = 0;
		virtual void FreeImpl(void* ptr) = 0;
		virtual void ZeroMemoryImpl(void* ptr, size_t size) = 0;
		virtual void CopyMemoryImpl(void* dest, const void* src, size_t size) = 0;
		virtual void SetMemoryImpl(void* ptr, int value, size_t size) = 0;

	private:
		static IPlatform* s_Instance;

	};

}