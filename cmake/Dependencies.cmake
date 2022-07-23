include(FetchContent)

FetchContent_Declare(dpCMake
    GIT_REPOSITORY https://github.com/GroPask/dpCMake.git
    GIT_TAG 0.0.2
)
FetchContent_MakeAvailable(dpCMake)

include(${dpcmake_SOURCE_DIR}/dpCMake.cmake)

find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL ${IMAPP_INSTALL} CACHE BOOL "" FORCE)

    FetchContent_Declare(glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.6
    )
    FetchContent_MakeAvailable(glfw3)
endif ()

find_package(imgui QUIET)
if (NOT imgui_FOUND)
    set(IMGUI_INSTALL ${IMAPP_INSTALL} CACHE BOOL "" FORCE)

    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 9cd9c2eff99877a3f10a7f9c2a3a5b9c15ea36c6
    )

    FetchContent_GetProperties(imgui)
    if(NOT imgui_POPULATED)
        FetchContent_Populate(imgui)

        file(COPY ${imgui_SOURCE_DIR}/backends/ DESTINATION ${CMAKE_BINARY_DIR}/ImGuiBackendsImpl)

        include(ImGuiCMakeGenerator.cmake)
        generateImGuiCMakeFiles(${imgui_SOURCE_DIR})

        add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
    endif ()
endif ()

find_package(OpenGL REQUIRED)
if (NOT TARGET opengl::opengl)
    add_library(opengl::opengl ALIAS OpenGL::GL) # Create fake Conan OpenGl pointing to CMake OpenGl
endif ()
