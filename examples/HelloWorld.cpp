#include <ImApp/ImApp.hpp>

int main()
{
    return ImApp::Run("ImApp_Example_HelloWorld", [&]()
    {
        ImGui::Text("Hello World");
    });
}
