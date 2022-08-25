#include <ImApp/ImApp.hpp>

int main()
{
    return ImApp::Run("MyApp", [&]()
    {
        if (ImApp::BeginMainWindowContent())
            ImGui::Text("Hello World");
        ImApp::EndMainWindowContent();

        ImGui::Begin("Other Window");
        ImGui::End();
    });
}
