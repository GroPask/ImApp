#include "ImApp/ImApp.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <cassert>
#include <cstdio>
#include <cstdlib>

namespace ImApp
{
    namespace details
    {
        void HideCloseButton(GLFWwindow& window) noexcept;
        void ShowCloseButton(GLFWwindow& window) noexcept;
    }
}

namespace
{
    struct ImAppContext
    {
        static void GlfwErrorCallback(int error, const char* description) noexcept;

        static constexpr ImVec4 ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        bool Init(const char* mainWindowTitle, ImApp::AppFlags appFlags) noexcept;

        bool PollEvents(bool* open) noexcept;
        void BeginFrame() noexcept;
        void EndFrame() noexcept;

        int Terminate() noexcept;

        void HideMainCloseButtonIfNeeded(bool* open) noexcept;
        void ShowMainCloseButtonIfNeeded(bool* open) noexcept;

        int NotInitTerminateFunc() noexcept;
        int OnlyGlfwInitTerminateFunc() noexcept;
        int StandardTerminateFunc() noexcept;

        int (ImAppContext::*terminateFunc)() noexcept = &ImAppContext::NotInitTerminateFunc;
        GLFWwindow* mainWindow = nullptr;
        void (ImAppContext::* manageMainCloseButtonFunc)(bool* open) noexcept = &ImAppContext::HideMainCloseButtonIfNeeded;
    };

    ImAppContext ImAppGlobalContext;

    void ImAppContext::GlfwErrorCallback(int error, const char* description) noexcept
    {
        std::fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    bool ImAppContext::Init(const char* mainWindowTitle, ImApp::AppFlags appFlags) noexcept
    {
        assert(mainWindow == nullptr);

        glfwSetErrorCallback(&ImAppContext::GlfwErrorCallback);
        if (!glfwInit())
            return false;

        terminateFunc = &ImAppContext::OnlyGlfwInitTerminateFunc;
        manageMainCloseButtonFunc = &ImAppContext::HideMainCloseButtonIfNeeded;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
        // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

        if (appFlags.Has(ImApp::AppFlag::MainWindow_NoResize))
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create window with graphics context
        mainWindow = glfwCreateWindow(1280, 720, mainWindowTitle, nullptr, nullptr);
        if (mainWindow == nullptr)
            return false;

        terminateFunc = &ImAppContext::StandardTerminateFunc;      

        glfwMakeContextCurrent(mainWindow);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);

        return true;
    }

    bool ImAppContext::PollEvents(bool* open) noexcept
    {
        assert(mainWindow != nullptr);

        if (open != nullptr)
        {
            if (glfwWindowShouldClose(mainWindow))
            {
                *open = false;
                glfwSetWindowShouldClose(mainWindow, GL_FALSE); // Just catch the event, don't want to persits next frame
                return false;
            }

            *open = true;
        }        

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        (this->*manageMainCloseButtonFunc)(open);

        return true;
    }

    void ImAppContext::BeginFrame() noexcept
    {       
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImAppContext::EndFrame() noexcept
    {
        assert(mainWindow != nullptr);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(mainWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(mainWindow);
    }

    int ImAppContext::Terminate() noexcept
    {
        return (this->*terminateFunc)();
    }

    void ImAppContext::HideMainCloseButtonIfNeeded(bool* open) noexcept
    {
        assert(mainWindow != nullptr);

        if (open != nullptr)
            return;

        ImApp::details::HideCloseButton(*mainWindow);

        manageMainCloseButtonFunc = &ImAppContext::ShowMainCloseButtonIfNeeded;
    }

    void ImAppContext::ShowMainCloseButtonIfNeeded(bool* open) noexcept
    {
        assert(mainWindow != nullptr);

        if (open == nullptr)
            return;

        ImApp::details::ShowCloseButton(*mainWindow);

        manageMainCloseButtonFunc = &ImAppContext::HideMainCloseButtonIfNeeded;
    }

    int ImAppContext::NotInitTerminateFunc() noexcept
    {
        return EXIT_FAILURE;
    }

    int ImAppContext::OnlyGlfwInitTerminateFunc() noexcept
    {
        assert(mainWindow == nullptr);

        glfwTerminate();

        return EXIT_FAILURE;
    }

    int ImAppContext::StandardTerminateFunc() noexcept
    {
        assert(mainWindow != nullptr);

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(mainWindow);
        glfwTerminate();

        terminateFunc = &ImAppContext::NotInitTerminateFunc;
        mainWindow = nullptr;

        return EXIT_SUCCESS;
    }
}

bool ImApp::Init(const char* mainWindowTitle, AppFlags appFlags) noexcept
{
    return ImAppGlobalContext.Init(mainWindowTitle, appFlags);
}

int ImApp::Terminate() noexcept
{
    return ImAppGlobalContext.Terminate();
}

bool ImApp::PollEvents(bool* open) noexcept
{
    return ImAppGlobalContext.PollEvents(open);
}

void ImApp::BeginFrame() noexcept
{
    ImAppGlobalContext.BeginFrame();
}

void ImApp::EndFrame() noexcept
{
    ImAppGlobalContext.EndFrame();
}

bool ImApp::BeginMainWindowContent() noexcept
{
    // Implementation found at https://github.com/ocornut/imgui/issues/3541

#ifdef IMGUI_HAS_VIEWPORT
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
#else 
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    return ImGui::Begin("ImAppMainWindowContent", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
}

void ImApp::EndMainWindowContent() noexcept
{
    ImGui::End();
    ImGui::PopStyleVar(1);
}
