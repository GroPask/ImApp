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
    set(GLFW_INSTALL ON CACHE BOOL "" FORCE)

    FetchContent_Declare(glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.6
    )
    FetchContent_MakeAvailable(glfw3)
endif ()

find_package(imgui QUIET)
if (NOT imgui_FOUND)
    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 9cd9c2eff99877a3f10a7f9c2a3a5b9c15ea36c6
    )
    FetchContent_GetProperties(imgui)
    if(NOT imgui_POPULATED)
        FetchContent_Populate(imgui)

        file(COPY ${imgui_SOURCE_DIR}/backends/ DESTINATION ${CMAKE_BINARY_DIR}/ImGuiBackendsImpl)

        file(DOWNLOAD
            https://raw.githubusercontent.com/conan-io/conan-center-index/5a7dc52e8ae19eead370591a6d2fa5e4953f0e68/recipes/imgui/all/CMakeLists.txt
            ${imgui_SOURCE_DIR}/CMakeLists.txt
        )

        file(READ ${imgui_SOURCE_DIR}/CMakeLists.txt imGuiCMakeListsContent)
        string(REPLACE "project(imgui" "project(imgui VERSION 1.88 LANGUAGES" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
        string(REPLACE "include(conanbuildinfo.cmake)" "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
        string(REPLACE "conan_basic_setup(TARGETS)" "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
        string(REPLACE "/source_subfolder" "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
        string(REPLACE [=[add_library(${PROJECT_NAME} ${SOURCE_FILES})]=] [=[add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})]=] imGuiCMakeListsContent "${imGuiCMakeListsContent}")
        string(REPLACE [=[install(TARGETS ${PROJECT_NAME}]=] [=[install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets]=] imGuiCMakeListsContent "${imGuiCMakeListsContent}")
        file(WRITE ${imgui_SOURCE_DIR}/CMakeLists.txt "${imGuiCMakeListsContent}")

        file(WRITE ${imgui_SOURCE_DIR}/ImGuiConfig.cmake.in [=[@PACKAGE_INIT@
include ( "${CMAKE_CURRENT_LIST_DIR}/ImGuiTargets.cmake" )
check_required_components(ImGui)
]=])

        file(APPEND ${imgui_SOURCE_DIR}/CMakeLists.txt [=[include(CMakePackageConfigHelpers)
configure_package_config_file(ImGuiConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfigVersion.cmake COMPATIBILITY SameMajorVersion)
install(EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Targets.cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
]=])

        add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
    endif ()
endif ()

find_package(OpenGL REQUIRED)
if (NOT TARGET opengl::opengl)
    add_library(opengl::opengl ALIAS OpenGL::GL) # Create fake Conan OpenGl pointing to CMake OpenGl
endif ()
