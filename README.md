# ImApp

## Introduction
ImApp is an Open Source library allowing you to easily write standalone [Dear ImGui](https://github.com/ocornut/imgui) application.

## Basic usage

##### C++
```c++
#include <ImApp/ImApp.hpp>

int main()
{
    return ImApp::Run("MyApp", [&]()
    {
        ImGui::Text("Hello World");
    });
}
```

##### CMake
```cmake
include(FetchContent)
FetchContent_Declare(ImApp URL https://github.com/GroPask/ImApp/archive/refs/tags/0.0.1.zip)
FetchContent_MakeAvailable(ImApp)

# ...

target_link_libraries(MyApp PRIVATE ImApp::ImApp)
```

## License

ImApp is licensed under the MIT License, see [LICENSE.txt](https://github.com/GroPask/ImApp/blob/main/LICENSE.txt) for more information.
