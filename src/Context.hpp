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
        bool Init(const char* mainWindowTitle, AppFlags appFlags) noexcept;

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

        unsigned long long m_frameCount;
        bool m_mainWindowSizeHasBeenLoaded;

        int (Context::* m_terminateFunc)() noexcept = &Context::NotInitTerminateFunc;
        void (Context::* m_manageMainCloseButtonFunc)(bool* open) noexcept;

        GLFWwindow* m_mainWindow = nullptr;
        bool m_mainWindowHasBeenResizedByUser;
        bool m_isCurrentlyResizingMainWindow;
    };
}

#endif
