#include <ImApp/ImApp.hpp>

int main()
{
    if (ImApp::Init("ImApp_Example_NoClose"))
    {
        for (bool open = true; open; )
        {
            ImApp::Update([&]()
            {
                if (ImGui::Button("Close"))
                    open = false;
            });
        }
    }

    return ImApp::Terminate();
}
