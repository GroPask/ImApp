#ifndef IMAPP_CONTEXT_HPP
#define IMAPP_CONTEXT_HPP

#include "ImApp/ImApp.hpp"

struct GLFWwindow;

namespace ImApp
{
    class Context
    {
    public:
        bool Init(const char* mainWindowTitle, ImApp::AppFlags appFlags) noexcept;

        bool PollEvents(bool* open) noexcept;
        void BeginFrame() noexcept;
        void EndFrame() noexcept;

        int Terminate() noexcept;

        bool BeginMainWindowContent() noexcept;
        void EndMainWindowContent() noexcept;

    private:
        void OnMainWindowResized();

        void HideMainCloseButtonIfNeeded(bool* open) noexcept;
        void ShowMainCloseButtonIfNeeded(bool* open) noexcept;

        int NotInitTerminateFunc() noexcept;
        int OnlyGlfwInitTerminateFunc() noexcept;
        int StandardTerminateFunc() noexcept;

        unsigned long long frameCount = 0;

        int (Context::* terminateFunc)() noexcept = &Context::NotInitTerminateFunc;
        void (Context::* manageMainCloseButtonFunc)(bool* open) noexcept = &Context::HideMainCloseButtonIfNeeded;

        GLFWwindow* mainWindow = nullptr;
        bool mainWindowHasBeenResizedByUser = false;
        bool currentlyResizeMainWindow = false;
    };
}

#endif
