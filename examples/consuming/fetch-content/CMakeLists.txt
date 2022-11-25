cmake_minimum_required(VERSION 3.21.0...3.25.0)

project(ImApp_ConsumingExample_FetchContent LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(ImApp URL https://github.com/GroPask/ImApp/archive/refs/tags/0.0.1.zip)
FetchContent_MakeAvailable(ImApp)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ImApp::ImApp)
