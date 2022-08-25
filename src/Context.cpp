#include "Context.hpp"

#include "SystemSpecific.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <imgui.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <algorithm>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#pragma warning(disable : 4996)

void ImApp::Context::SetNextMainWindowSize(int witdh, int height, Cond cond) noexcept
{
    m_mainWindowSizeHasBeenSet = true;
    m_mainWindowWidth = witdh;
    m_mainWindowHeight = height;    
    m_mainWindowSizeCond = cond;
}

bool ImApp::Context::Init(const char* mainWindowTitle, AppFlags appFlags) noexcept
{
    assert(m_mainWindow == nullptr);

    glfwSetErrorCallback([](int error, const char* description)
    {
        std::fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    });

    if (!glfwInit())
        return false;

    m_terminateFunc = &Context::OnlyGlfwInitTerminateFunc;

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
    if (m_mainWindowSizeHasBeenSet)
        m_mainWindow = glfwCreateWindow(m_mainWindowWidth, m_mainWindowHeight, mainWindowTitle, nullptr, nullptr);
    else
        m_mainWindow = glfwCreateWindow(1280, 720, mainWindowTitle, nullptr, nullptr);        

    if (m_mainWindow == nullptr)
        return false;

    m_appFlags = appFlags;
    mainWindowTitleLen = static_cast<int>(std::strlen(mainWindowTitle));
    m_frameCount = 0;
    m_mainWindowSizeHasBeenLoaded = false;
    m_terminateFunc = &Context::StandardTerminateFunc;
    m_manageMainCloseButtonFunc = &Context::HideMainCloseButtonIfNeeded;
    m_mainWindowHasBeenResizedByUser = false;
    m_isCurrentlyResizingMainWindow = false;

    glfwSetWindowUserPointer(m_mainWindow, this);

    glfwSetWindowSizeCallback(m_mainWindow, [](GLFWwindow* window, int /*width*/, int /*height*/)
    {
        assert(window != nullptr);

        ImApp::Context* context = static_cast<ImApp::Context*>(glfwGetWindowUserPointer(window));
        assert(context != nullptr);

        context->OnMainWindowResized();
    });

    glfwMakeContextCurrent(m_mainWindow);
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

    static constexpr const char* imAppSaveDataName = "ImApp";
    static constexpr const char* imAppMainSaveDataEntryName = "MainData";

    ImGuiSettingsHandler settingsHandler;
    settingsHandler.TypeName = imAppSaveDataName;
    settingsHandler.TypeHash = ImHashStr(imAppSaveDataName);
    settingsHandler.ReadOpenFn = [](ImGuiContext* /*ctx*/, ImGuiSettingsHandler* /*handler*/, const char* name) -> void*
    {
        if (std::strcmp(name, imAppMainSaveDataEntryName) == 0)
            return reinterpret_cast<void*>(1);

        return nullptr;
    };
    settingsHandler.ReadLineFn = [](ImGuiContext* /*ctx*/, ImGuiSettingsHandler* handler, void* /*entry*/, const char* line)
    {
        Context* context = static_cast<Context*>(handler->UserData);
        assert(context != nullptr);

        context->ReadMainSaveDataLine(line);
    };
    settingsHandler.WriteAllFn = [](ImGuiContext* /*ctx*/, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf)
    {
        Context* context = static_cast<Context*>(handler->UserData);
        assert(context != nullptr);

        out_buf->appendf("[%s][%s]\n", handler->TypeName, imAppMainSaveDataEntryName);

        context->WriteAllMainSaveData(*out_buf);

        out_buf->appendf("\n");
    };
    settingsHandler.UserData = this;
    ImGui::AddSettingsHandler(&settingsHandler);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_mainWindow, true);
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

bool ImApp::Context::PollEvents(bool* open) noexcept
{
    assert(m_mainWindow != nullptr);

    if (open != nullptr)
    {
        if (glfwWindowShouldClose(m_mainWindow))
        {
            *open = false;
            glfwSetWindowShouldClose(m_mainWindow, GL_FALSE); // Just catch the event, don't want to persits next frame
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

    (this->*m_manageMainCloseButtonFunc)(open);

    return true;
}

void ImApp::Context::BeginFrame() noexcept
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImApp::Context::EndFrame() noexcept
{
    assert(m_mainWindow != nullptr);

    static constexpr ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(m_mainWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clearColor.x * clearColor.w, clearColor.y * clearColor.w, clearColor.z * clearColor.w, clearColor.w);
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

    glfwSwapBuffers(m_mainWindow);

    ++m_frameCount;
}

int ImApp::Context::Terminate() noexcept
{
    return (this->*m_terminateFunc)();
}

bool ImApp::Context::BeginMainWindowContent() noexcept
{
    assert(m_mainWindow != nullptr);

    // Implementation found at https://github.com/ocornut/imgui/issues/3541

#ifdef IMGUI_HAS_VIEWPORT
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    if (m_frameCount >= 2 || !CanMakeMainWindowSizeFitContent())
        ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
#else 
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    if (m_frameCount >= 2 || !CanMakeMainWindowSizeFitContent())
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("ImAppMainWindowContent", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

    const int mainWindowIsIconified = glfwGetWindowAttrib(m_mainWindow, GLFW_ICONIFIED);
    return !static_cast<bool>(mainWindowIsIconified);
}

void ImApp::Context::EndMainWindowContent() noexcept
{
    assert(m_mainWindow != nullptr);

    if (m_frameCount == 1 && CanMakeMainWindowSizeFitContent())
    {
        static constexpr int titleBarButtonsWidthHeuristic = 215;
        static constexpr int titleBarCharacherWithHeuristic = 8;

        const ImVec2 mainWindowContentSize = ImGui::GetWindowSize();

        const int mainWindowMinWidth = static_cast<int>(mainWindowTitleLen) * titleBarCharacherWithHeuristic + titleBarButtonsWidthHeuristic;
        const int mainWindowWantedWidth = std::max(static_cast<int>(mainWindowContentSize.x), mainWindowMinWidth);
        const int mainWindowWantedHeight = static_cast<int>(mainWindowContentSize.y);

        SetMainWindowSize(mainWindowWantedWidth, mainWindowWantedHeight);
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
}

void ImApp::Context::OnMainWindowResized() noexcept
{
    if (!m_isCurrentlyResizingMainWindow)
    {
        m_mainWindowHasBeenResizedByUser = true;

        glfwSetWindowSizeCallback(m_mainWindow, nullptr); // We don't need callback anymore
    }
}

void ImApp::Context::SetMainWindowSize(int width, int height) noexcept
{
    m_isCurrentlyResizingMainWindow = true;

    glfwSetWindowSize(m_mainWindow, width, height);

    m_isCurrentlyResizingMainWindow = false;
}

bool ImApp::Context::CanMakeMainWindowSizeFitContent() const noexcept
{
    return (!m_mainWindowSizeHasBeenSet && !m_mainWindowSizeHasBeenLoaded);
}

void ImApp::Context::ReadMainSaveDataLine(const char* line) noexcept
{
    assert(line != nullptr);
    assert(m_mainWindow != nullptr);

    int mainWindowX;
    int mainWindowY;
    if (std::sscanf(line, "MainWindowPos=%d,%d\n", &mainWindowX, &mainWindowY) == 2)
    {
        glfwSetWindowPos(m_mainWindow, mainWindowX, mainWindowY);
        return;
    }

    if (!m_appFlags.HasOneOf(AppFlag::MainWindow_NoSaveSize |
                             AppFlag::MainWindow_NoResize) && // Force ignore maybe previously saved size (with a version of this app without NoResize flag)
        (!m_mainWindowSizeHasBeenSet || m_mainWindowSizeCond != Cond::Always)) // If MainWindowSize as been setted and Cond == Always, no needs to load size because we want to use the size given by the user at each new app launch
    {
        int mainWindowWidth;
        int mainWindowHeight;
        if (std::sscanf(line, "MainWindowSize=%d,%d\n", &mainWindowWidth, &mainWindowHeight) == 2)
        {
            m_mainWindowSizeHasBeenLoaded = true;
            SetMainWindowSize(mainWindowWidth, mainWindowHeight);

            return;
        }
    }
}

void ImApp::Context::WriteAllMainSaveData(ImGuiTextBuffer& textBuffer) const noexcept
{
    assert(m_mainWindow != nullptr);

    int mainWindowX;
    int mainWindowY;
    glfwGetWindowPos(m_mainWindow, &mainWindowX, &mainWindowY);

    textBuffer.appendf("MainWindowPos=%d,%d\n", mainWindowX, mainWindowY);

    if (!m_appFlags.HasOneOf(AppFlag::MainWindow_NoSaveSize) &&
        (m_mainWindowSizeHasBeenLoaded || m_mainWindowHasBeenResizedByUser) &&
        (!m_mainWindowSizeHasBeenSet || m_mainWindowSizeCond != Cond::Always)) // If MainWindowSize as been setted and Cond == Always, no needs to save size because we want to use the size given by the user at each new app launch
    {
        int mainWindowWidth;
        int mainWindowHeight;
        glfwGetWindowSize(m_mainWindow, &mainWindowWidth, &mainWindowHeight);

        textBuffer.appendf("MainWindowSize=%d,%d\n", mainWindowWidth, mainWindowHeight);
    }
}

void ImApp::Context::HideMainCloseButtonIfNeeded(bool* open) noexcept
{
    assert(m_mainWindow != nullptr);

    if (open != nullptr)
        return;

    HideCloseButton(*m_mainWindow);

    m_manageMainCloseButtonFunc = &Context::ShowMainCloseButtonIfNeeded;
}

void ImApp::Context::ShowMainCloseButtonIfNeeded(bool* open) noexcept
{
    assert(m_mainWindow != nullptr);

    if (open == nullptr)
        return;

    ShowCloseButton(*m_mainWindow);

    m_manageMainCloseButtonFunc = &Context::HideMainCloseButtonIfNeeded;
}

int ImApp::Context::NotInitTerminateFunc() noexcept
{
    return EXIT_FAILURE;
}

int ImApp::Context::OnlyGlfwInitTerminateFunc() noexcept
{
    assert(m_mainWindow == nullptr);

    glfwTerminate();

    return EXIT_FAILURE;
}

int ImApp::Context::StandardTerminateFunc() noexcept
{
    assert(m_mainWindow != nullptr);

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_mainWindow);
    glfwTerminate();

    m_mainWindowSizeHasBeenSet = false;
    m_terminateFunc = &Context::NotInitTerminateFunc;
    m_mainWindow = nullptr;

    return EXIT_SUCCESS;
}
