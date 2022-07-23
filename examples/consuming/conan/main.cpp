#include <ImApp/ImApp.hpp>

int main()
{
    ImApp::Application app(1280, 720, "ImApp_ConsumingExample_Conan");

    return app.run([&]()
    {
        ImGui::Text("Hello World");

        if (ImGui::Button("Close"))
            app.close();
    });
}
