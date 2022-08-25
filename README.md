# ImApp

## Introduction
ImApp is an Open Source library allowing you to easily write standalone [Dear ImGui](https://github.com/ocornut/imgui) application.

## Basic usage

### C++

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
### CMake
```cmake
project(MyApp)

include(FetchContent)
FetchContent_Declare(ImApp GIT_REPOSITORY https://github.com/GroPask/ImApp.git GIT_TAG main)
FetchContent_MakeAvailable(ImApp)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE ImApp::ImApp)
```
