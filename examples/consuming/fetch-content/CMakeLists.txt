cmake_minimum_required(VERSION 3.16.0)

project(ImApp_ConsumingExample_CMakeInstall LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(ImApp
  GIT_REPOSITORY https://github.com/GroPask/ImApp.git
  GIT_TAG main
)
FetchContent_MakeAvailable(ImApp)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ImApp::ImApp)
