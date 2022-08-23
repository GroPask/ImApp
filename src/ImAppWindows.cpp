#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <Windows.h>

int main(int argc, char** argv);

int WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PSTR /*lpCmdLine*/, INT /*nCmdShow*/)
{
    return main(__argc, __argv);
}

namespace ImApp
{
    namespace details
    {
        void HideCloseButton(GLFWwindow& window) noexcept
        {
            const HWND hwnd = glfwGetWin32Window(&window);
            EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        }

        void ShowCloseButton(GLFWwindow& window) noexcept
        {
            const HWND hwnd = glfwGetWin32Window(&window);
            EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
        }
    }
}
