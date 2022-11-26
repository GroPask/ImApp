#include "ImApp/ImApp.hpp"

#include "Context.hpp"

namespace
{
    ImApp::Context ImAppGlobalContext;
}

bool ImApp::details::PollEvents(bool* open) noexcept
{
    return ImAppGlobalContext.PollEvents(open);
}

void ImApp::details::BeginFrame() noexcept
{
    ImAppGlobalContext.BeginFrame();
}

void ImApp::details::EndFrame() noexcept
{
    ImAppGlobalContext.EndFrame();
}

bool ImApp::Init(const char* mainWindowTitle, AppFlags appFlags) noexcept
{
    return ImAppGlobalContext.Init(mainWindowTitle, appFlags);
}

int ImApp::Terminate() noexcept
{
    return ImAppGlobalContext.Terminate();
}

void ImApp::SetNextMainWindowSize(int witdh, int height, Cond cond) noexcept
{
    ImAppGlobalContext.SetNextMainWindowSize(witdh, height, cond);
}

bool ImApp::BeginMainWindowContent(MainWindowContentFlags mainWindowContentFlags) noexcept
{
    return ImAppGlobalContext.BeginMainWindowContent(mainWindowContentFlags);
}

void ImApp::EndMainWindowContent() noexcept
{
    ImAppGlobalContext.EndMainWindowContent();
}
