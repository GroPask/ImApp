#include <ImApp/ImApp.hpp>

int main(int, char**)
{
    ImApp::Application app(1280, 720, "ImApp_ConsumingExample_FetchContent");

    return app.run([&]()
    {
        ImGui::Text("Hello World");

        if (ImGui::Button("Close"))
            app.close();
    });
}