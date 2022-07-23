#include <ImApp/ImApp.hpp>

int main(int, char**)
{
    ImApp::Application app(1280, 720, "ImApp_Test_Simple");

    return app.run([&]()
    {
        ImGui::Text("Hello World");

        if (ImGui::Button("Close"))
            app.close();
    });
}
