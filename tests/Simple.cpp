#include <ImApp/ImApp.hpp>

int main(int /*argc*/, char** /*argv*/)
{
    ImApp::Application app(1280, 720, "ImApp_Test_Simple");

    return app.run([&]()
    {
        if (ImGui::Begin("WindowA"))
        {
            if (ImGui::Button("Close"))
                app.close();
        }

        ImGui::End();
    });
}
