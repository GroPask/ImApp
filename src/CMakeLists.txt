set(srcDir .)
set(incDir ../include/ImApp)

target_sources(${PROJECT_NAME} PRIVATE CMakeLists.txt
    ${srcDir}/ImApp.cpp
    ${incDir}/ImApp.hpp
    ${incDir}/ImApp.inl
    ${srcDir}/ImGuiImplGlfw.cpp
    ${srcDir}/ImGuiImplOpenGl3.cpp
)

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        ${srcDir}/WinMain.cpp
    )
endif ()
