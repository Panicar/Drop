#pragma once

#include "Application/Application.hpp"
#include <memory>

// Platform detection
#ifdef _WIN32
#ifdef _WIN64
#define DP_PLATFORM_WINDOWS
#else
#define DP_PLATFORM_WINDOWS
#endif
#endif

// Entry point definition
#if defined(DP_PLATFORM_WINDOWS) && defined(DP_MASTER_MODE)
#include <Windows.h>
#define DP_MAIN_ENTRY int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#define DP_MAIN_ENTRY int main(int argc, char** argv)
#endif

namespace DP {

    // Forward declaration of application creation function
    extern "C" CoreApplication* CreateApplication(int argc, char** argv);

    inline int FrameworkMain(int argc, char** argv)
    {
        // Platform::Initialize();  // Uncomment when platform layer is ready

        // Create user application
        std::unique_ptr<CoreApplication> app(CreateApplication(argc, argv));

        // Initialize and run application
        app->Execute();

        // Platform::Shutdown();
        return 0;
    }

} // namespace DP

// Application definition macro
#define DP_DEFINE_APPLICATION(AppClass) \
    extern "C" DP::CoreApplication* CreateApplication(int argc, char** argv) { \
        return new AppClass(/*argc, argv*/); \
    }

// Main entry point macro
#define DP_IMPLEMENT_APPLICATION \
    DP_MAIN_ENTRY { \
            return DP::FrameworkMain(argc, argv); \
    }