#pragma once

#include "Platform/Platform.hpp"
#include "Log/Log.hpp"
#include "Application/Application.hpp"
#include "Application/AppConfig.hpp"
#include <memory>


namespace drop {

    // Forward declaration: user must define this in their application
    extern "C" CoreApplication* CreateApplication();

    inline int Main(IAppConfig* config)
    {
        Log::Initialize();
        IPlatform::Initialize();

        CoreApplication* app = CreateApplication();

        if (!app) 
        {
            DP_CORE_CRITICAL("Failed to create application instance!");
            IPlatform::Terminate();
            Log::Shutdown();
            return -1;
        }

        auto appConfig = static_cast<ConsoleAppConfig*>(config);
        std::string args;
        for (int i = 0; i < appConfig->argc; ++i) {
            args += std::string(appConfig->argv[i]) + " ";
        }
        DP_CORE_CRITICAL("Argc: {}, Argv: {}", appConfig->argc, args);

        app->Execute();
        delete app;

        IPlatform::Terminate();
        Log::Shutdown();
        return 0;
    }

} // namespace drop

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
int main(int argc, char** argv)                                                                     \
{                                                                                                   \
    auto config = new drop::ConsoleAppConfig;                                                       \
    config->argc = argc;                                                                            \
    config->argv = argv;                                                                            \
    int result = drop::Main(config);                                                                \
    delete config;                                                                                  \
    return result;                                                                                  \
}
#endif

#define DP_IMPLEMENT_ENTRY_POINT MAIN;
    


#define DP_IMPLEMENT_APPLICATION(AppClass)                                              \
    extern "C" drop::CoreApplication* CreateApplication()                               \
    {                                                                                   \
        return new AppClass();                                                          \
    }                                                                                   \
                                                                                        \
    DP_IMPLEMENT_ENTRY_POINT;                                                           \
    
