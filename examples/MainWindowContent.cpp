#include <ImApp/ImApp.hpp>

int main()
{
    return ImApp::Run("ImApp_Example_MainWindowContent", [&]()
    {
        if (ImApp::BeginMainWindowContent())
            ImGui::Text("Hello World");

        ImApp::EndMainWindowContent();
    });
}
