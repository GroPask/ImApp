function(generateImGuiCMakeFiles imGuiSrcDir)
    file(DOWNLOAD
        https://raw.githubusercontent.com/conan-io/conan-center-index/5a7dc52e8ae19eead370591a6d2fa5e4953f0e68/recipes/imgui/all/CMakeLists.txt
        ${imGuiSrcDir}/CMakeLists.txt
    )

    file(READ ${imGuiSrcDir}/CMakeLists.txt imGuiCMakeListsContent)
    string(REPLACE "project(imgui" "project(imgui VERSION 1.88 LANGUAGES" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE "include(conanbuildinfo.cmake)" "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE "conan_basic_setup(TARGETS)" "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE "/source_subfolder" "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE [=[add_executable(${BINARY_TO_COMPRESSED_BIN} ${EXTRA_FONTS_DIR}/binary_to_compressed_c.cpp)]=] "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE [=[add_library(${PROJECT_NAME} ${SOURCE_FILES})]=] [=[add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})]=] imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE "include(GNUInstallDirs)" [=[option(IMGUI_INSTALL "Generate the install target" ON)
if (IMGUI_INSTALL)
include(GNUInstallDirs)]=] imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE [=[install(TARGETS ${PROJECT_NAME}]=] [=[install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets]=] imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    string(REPLACE [=[install(TARGETS ${BINARY_TO_COMPRESSED_BIN}
        DESTINATION ${CMAKE_INSTALL_BINDIR})]=] "" imGuiCMakeListsContent "${imGuiCMakeListsContent}")
    file(WRITE ${imGuiSrcDir}/CMakeLists.txt "${imGuiCMakeListsContent}")

    file(APPEND ${imGuiSrcDir}/CMakeLists.txt [=[include(CMakePackageConfigHelpers)

configure_package_config_file(ImGuiConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfigVersion.cmake COMPATIBILITY SameMajorVersion)

install(EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Targets.cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/ImGuiConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

endif (IMGUI_INSTALL)
]=])

    file(WRITE ${imGuiSrcDir}/ImGuiConfig.cmake.in [=[@PACKAGE_INIT@
include ( "${CMAKE_CURRENT_LIST_DIR}/ImGuiTargets.cmake" )
check_required_components(ImGui)
]=])
endfunction ()
