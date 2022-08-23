#include <ImApp/ImApp.hpp>

int main()
{
    return ImApp::Run("ImApp_ConsumingExample_Conan", [&]()
    {
        ImGui::Text("Hello World");

        if (ImGui::Button("Close"))
            return false;

        return true;
    });
}
