function (configureGlfw)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
    set(GLFW_INSTALL ${IMAPP_INSTALL} CACHE BOOL "")
endfunction ()

find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
    dp_download_and_add_dependency(
        URL https://github.com/glfw/glfw/archive/refs/tags/3.3.8.zip
        CONFIGURE_FUNC configureGlfw
    )
endif ()

function (patchImgui imguiSrcDir)
    file(DOWNLOAD
        https://raw.githubusercontent.com/conan-io/conan-center-index/5a7dc52e8ae19eead370591a6d2fa5e4953f0e68/recipes/imgui/all/CMakeLists.txt
        ${imguiSrcDir}/CMakeLists.txt
    )

    dp_patch_file(${imguiSrcDir}/CMakeLists.txt
        ADD_AFTER "project(imgui" " VERSION 1.88 LANGUAGES"
        REMOVE "include(conanbuildinfo.cmake)"
        REMOVE "conan_basic_setup(TARGETS)"
        REMOVE "/source_subfolder"
        REMOVE [=[add_executable(${BINARY_TO_COMPRESSED_BIN} ${EXTRA_FONTS_DIR}/binary_to_compressed_c.cpp)]=]
        ADD_AFTER [=[add_library(${PROJECT_NAME} ${SOURCE_FILES}]=] [=[ ${HEADER_FILES}]=]
        ADD_LINE_BEFORE "set_target_properties" [=[add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})]=]
        ADD_LINE_BEFORE "include(GNUInstallDirs)" [=[option(IMGUI_INSTALL "Generate the install target" ON)]=]
        ADD_LINE_BEFORE "include(GNUInstallDirs)" "if (IMGUI_INSTALL)"
        ADD_AFTER [=[install(TARGETS ${PROJECT_NAME}]=] [=[ EXPORT ${PROJECT_NAME}Targets]=]
        REMOVE [=[install(TARGETS ${BINARY_TO_COMPRESSED_BIN}]=]
        REMOVE [=[DESTINATION ${CMAKE_INSTALL_BINDIR})]=]
        APPEND_LINE "include(CMakePackageConfigHelpers)"
        APPEND_LINE [=[configure_package_config_file(ImGuiConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})]=]
        APPEND_LINE [=[write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfigVersion.cmake COMPATIBILITY SameMajorVersion)]=]
        APPEND_LINE [=[install(EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Targets.cmake)]=]
        APPEND_LINE [=[install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})]=]
        APPEND_LINE "endif (IMGUI_INSTALL)"
    )
    
    dp_patch_file(${imguiSrcDir}/ImGuiConfig.cmake.in
        APPEND_LINE "@PACKAGE_INIT@"
        APPEND_LINE [=[include ( "${CMAKE_CURRENT_LIST_DIR}/ImGuiTargets.cmake" )]=]
        APPEND_LINE "check_required_components(ImGui)"
    )
endfunction ()

function (configureImgui)
    set(IMGUI_INSTALL ${IMAPP_INSTALL} CACHE BOOL "")
endfunction ()

find_package(imgui QUIET)
if (NOT imgui_FOUND)
    dp_download_and_add_dependency(
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG 9cd9c2eff99877a3f10a7f9c2a3a5b9c15ea36c6 
        PATCH_FUNC patchImgui
        CONFIGURE_FUNC configureImgui
        ALREADY_POPULATED_VAR imguiWasAlreadyPopulated
        SRC_DIR_VAR imguiSrcDir
    )

    if (NOT ${imguiWasAlreadyPopulated})
        file(COPY ${imguiSrcDir}/backends/ DESTINATION ${CMAKE_BINARY_DIR}/ImGuiBackendsImpl)
    endif ()
endif ()

find_package(OpenGL REQUIRED)
if (NOT TARGET opengl::opengl)
    add_library(opengl::opengl ALIAS OpenGL::GL) # Create fake Conan OpenGl pointing to CMake OpenGl
endif ()
