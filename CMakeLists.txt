cmake_minimum_required(VERSION 3.16.0)

project(ImApp VERSION 0.0.1 LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(IMAPP_MASTER_PROJECT TRUE)
endif ()

option(IMAPP_BUILD_EXAMPLES "Build examples programs" ${IMAPP_MASTER_PROJECT})
option(IMAPP_BUILD_TESTS "Build tests programs" ${IMAPP_MASTER_PROJECT})
option(IMAPP_INSTALL "Generate the install target" ${IMAPP_MASTER_PROJECT})

include(cmake/Dependencies.cmake)

dp_configure_global(
    USE_FOLDERS
    DEFAULT_CONFIGURATIONS Debug Release
)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/ImGuiBackendsImpl>
)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui PRIVATE glfw opengl::opengl)
target_sources(${PROJECT_NAME} PRIVATE conanfile.txt)

add_subdirectory(cmake)
add_subdirectory(src)

if (IMAPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (IMAPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

dp_configure_target(${PROJECT_NAME}
    DP_WARNINGS
    AUTO_SOURCE_GROUP
)

if (IMAPP_INSTALL)
    set(exportDestDir ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    set(imAppConfigOut ${CMAKE_CURRENT_BINARY_DIR}/ImAppConfig.cmake)
    set(imAppVersionOut ${CMAKE_CURRENT_BINARY_DIR}/ImAppConfigVersion.cmake)

    include(CMakePackageConfigHelpers) 
    configure_package_config_file(cmake/ImAppConfig.cmake.in ${imAppConfigOut} INSTALL_DESTINATION ${exportDestDir})
    write_basic_package_version_file(${imAppVersionOut} COMPATIBILITY SameMajorVersion)

    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/)
    install(EXPORT ${PROJECT_NAME}Targets DESTINATION ${exportDestDir} NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Targets.cmake)    
    install(FILES ${imAppConfigOut} ${imAppVersionOut} DESTINATION ${exportDestDir})
endif ()
