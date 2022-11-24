cmake_minimum_required(VERSION 3.16.0...3.25.0)

project(ImApp_ConsumingExample_CMakeInstall LANGUAGES CXX)

find_package(ImApp REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ImApp::ImApp)

