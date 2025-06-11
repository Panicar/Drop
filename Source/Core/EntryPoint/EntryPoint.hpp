#pragma once

#include "Log/Log.hpp"
#include "Application/Application.hpp"
#include <memory>

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
        // Platform::Initialize();

        Log::Initialize();

        DP_CORE_ERROR("Hello");
        DP_APP_CRITICAL("Hello 24124");

        std::unique_ptr<CoreApplication> app(CreateApplication(argc, argv));

        if (!app)
        {
            DP_CORE_CRITICAL("Failed to create application instance!");
            return -1;
        }

        app->Execute();
        
        Log::Shutdown();

        // Platform::Shutdown();
        return 0;
    }

} // namespace DP

#define DP_DEFINE_APPLICATION(AppClass)                                         \
    extern "C" DP::CoreApplication* CreateApplication(int argc, char** argv) {   \
        return new AppClass(/*argc, argv*/);                                      \
    }

#define DP_IMPLEMENT_APPLICATION                                                    \
    DP_MAIN_ENTRY {                                                                  \
            return DP::FrameworkMain(argc, argv);                                     \
    }