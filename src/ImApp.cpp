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

bool ImApp::BeginMainWindowContent() noexcept
{
    return ImAppGlobalContext.BeginMainWindowContent();
}

void ImApp::EndMainWindowContent() noexcept
{
    ImAppGlobalContext.EndMainWindowContent();
}
