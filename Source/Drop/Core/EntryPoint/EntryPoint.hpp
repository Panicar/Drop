#pragma once

#include "Platform/Platform.hpp"
#include "Log/Log.hpp"
#include "App/App.hpp"
#include "App/AppConfig.hpp"
#include <memory>

namespace drop {

    // Forward declaration: user must define this in their App
    extern "C" CoreApp* CreateApp();

    inline int Main(IAppConfig* config)
    {
        Log::Initialize();
        IPlatform::Initialize();

        CoreApp* app = CreateApp();
        
        if (!app) 
        {
            DP_CORE_CRITICAL("Failed to create App instance!");
            IPlatform::Terminate();
            Log::Shutdown();
            return -1;
        }

#ifdef DP_DEV_MODE
        auto appConfig = static_cast<ConsoleAppConfig*>(config);
        std::string args;
        for (int i = 0; i < appConfig->argc; ++i) {
            args += std::string(appConfig->argv[i]) + " ";
        }
        DP_CORE_WARN("Argc: {}, Argv: {}", appConfig->argc, args);
#endif
 
        app->Execute();
        delete app;

        IPlatform::Terminate();
        Log::Shutdown();
        return 0;
    }

} // namespace drop

// TODO: switch to std::unique_ptr
#if defined(DP_PLATFORM_WINDOWS) && defined(DP_MASTER_MODE)
#define MAIN \
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)   \
{                                                                                                   \
    auto config = new drop::WinAppConfig;                                                           \
    config->hInstance = hInstance;                                                                  \
    config->hPrevInstance = hPrevInstance;                                                          \
    config->lpCmdLine = lpCmdLine;                                                                  \
    config->nCmdShow = nCmdShow;                                                                    \
    int result = drop::Main(config);                                                                \
    delete config;                                                                                  \
    return result;                                                                                  \
}
#else
#define MAIN \
int main(int argc, char** argv)                         \
{                                                       \
    auto config = new drop::ConsoleAppConfig;           \
    config->argc = argc;                                \
    config->argv = argv;                                \
    int result = drop::Main(config);                    \
    delete config;                                      \
    return result;                                      \
}
#endif

#define DP_IMPLEMENT_ENTRY_POINT MAIN;

#define DP_IMPLEMENT_APP(AppClass)                      \
    extern "C" drop::CoreApp* CreateApp()               \
    {                                                   \
        return new AppClass();                          \
    }                                                   \
                                                        \
    DP_IMPLEMENT_ENTRY_POINT;                           \
    
