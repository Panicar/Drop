#pragma once

#include "Platform/Base/Platform.hpp"
#include "Log/Log.hpp"
#include "App/App.hpp"

#if defined(DP_PLATFORM_WINDOWS)
#include <shellapi.h>

#if defined(DP_MASTER_MODE)
#define MAIN int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#define ARGS HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
#define PASS_ARGS hInstance, hPrevInstance, lpCmdLine, nCmdShow
#else
#define MAIN int main(int argc, char** argv)
#define ARGS int argc, char** argv
#define PASS_ARGS argc, argv
#endif

extern drop::CoreApp* drop::CreateApp(const drop::CommandLineArgs& args);

namespace drop
{
    int Main(ARGS)
    {
        Log::Initialize();

#if defined(DP_MASTER_MODE)
        IPlatform::Create(hInstance);
#else
        IPlatform::Create(GetModuleHandleA(0));
#endif
        IPlatform::Initialize();

        CommandLineArgs commandLineArgs = {};

#if defined(DP_MASTER_MODE)
        LPWSTR cmdLine = GetCommandLineW();
        int argc;
        LPWSTR* argv = CommandLineToArgvW(cmdLine, &argc);

        commandLineArgs.Argc = argc;
        commandLineArgs.Argv = new char* [argc];
        for (int i = 0; i < argc; i++) {
            int size = WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, nullptr, 0, nullptr, nullptr);
            commandLineArgs.Argv[i] = new char[size];
            WideCharToMultiByte(CP_UTF8, 0, argv[i], -1, commandLineArgs.Argv[i], size, nullptr, nullptr);
        }
        LocalFree(argv);
#else

        commandLineArgs.Argc = argc;
        commandLineArgs.Argv = argv;
#endif
        CoreApp* app = CreateApp(commandLineArgs);

        if (!app)
        {
            DP_CORE_CRITICAL("Failed to create App instance!");
            IPlatform::Terminate();
            IPlatform::Destroy();
            Log::Shutdown();

#if defined(DP_MASTER_MODE)
            for (int i = 0; i < commandLineArgs.Argc; ++i)
                delete[] commandLineArgs.Argv[i];
            delete[] commandLineArgs.Argv;
#endif
            return -1;
        }

        app->Run();
        delete app;

#if defined(DP_MASTER_MODE)
        for (int i = 0; i < commandLineArgs.Argc; ++i) {
            delete[] commandLineArgs.Argv[i];
        }
        delete[] commandLineArgs.Argv;
#endif

        IPlatform::Terminate();
        IPlatform::Destroy();

        Log::Shutdown();
        return 0;

    }
} // namespace drop

#endif // defined Windows

#if defined(DP_PLATFORM_LINUX) || defined(DP_PLATFORM_MACOS)
#define MAIN int main(int argc, char** argv)
#define ARGS int argc, char** argv
#define PASS_ARGS argc, argv

namespace drop
{
    int Main(ARGS)
    {
        DP_CORE_INFO("Unix");

        return 0;
    }
}

#endif

#ifndef MAIN
#error MAIN is not defined
#endif

#define DP_IMPLEMENT_MAIN               \
    MAIN                                \
    {                                   \
        return drop::Main(PASS_ARGS);   \
    }

#define DP_IMPLEMENT_APP(AppClass)                                              \
    drop::CoreApp* drop::CreateApp(const drop::CommandLineArgs& args)           \
    {                                                                           \
        return new AppClass(args);                                              \
    }                                                                           \
    DP_IMPLEMENT_MAIN;                                                          \
