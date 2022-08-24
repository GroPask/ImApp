#ifndef IMAPP_CONTEXT_HPP
#define IMAPP_CONTEXT_HPP

#include "ImApp/ImApp.hpp"

struct ImGuiTextBuffer;
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
        void OnMainWindowResized() noexcept;

        void ReadMainSaveDataLine(const char* line) noexcept;
        void WriteAllMainSaveData(ImGuiTextBuffer& textBuffer) const noexcept;

        void HideMainCloseButtonIfNeeded(bool* open) noexcept;
        void ShowMainCloseButtonIfNeeded(bool* open) noexcept;

        int NotInitTerminateFunc() noexcept;
        int OnlyGlfwInitTerminateFunc() noexcept;
        int StandardTerminateFunc() noexcept;

        AppFlags m_appFlags;

        unsigned long long m_frameCount = 0;

        bool m_mainWindowSizeHasBeenLoaded = false;
        int m_mainWindowLoadedWidth = 0;
        int m_mainWindowLoadedHeight = 0;

        int (Context::* m_terminateFunc)() noexcept = &Context::NotInitTerminateFunc;
        void (Context::* m_manageMainCloseButtonFunc)(bool* open) noexcept = &Context::HideMainCloseButtonIfNeeded;

        GLFWwindow* m_mainWindow = nullptr;
        bool m_mainWindowHasBeenResizedByUser = false;
        bool m_isCurrentlyResizingMainWindow = false;
    };
}

#endif
