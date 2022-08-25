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
cmake_minimum_required(VERSION 3.16.0)

project(MyApp)

include(FetchContent)
FetchContent_Declare(ImApp GIT_REPOSITORY https://github.com/GroPask/ImApp.git GIT_TAG main)
FetchContent_MakeAvailable(ImApp)

add_executable(MyApp main.cpp)
target_link_libraries(MyApp PRIVATE ImApp::ImApp)
```

## License

ImApp is licensed under the MIT License, see [LICENSE.txt](https://github.com/GroPask/ImApp/blob/main/LICENSE.txt) for more information.
