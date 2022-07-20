set(srcDir .)
set(incDir ../include/ImApp)

target_sources(${PROJECT_NAME} PRIVATE CMakeLists.txt
    ${srcDir}/Application.cpp
    ${incDir}/Application.hpp
    ${incDir}/ImApp.hpp
    ${srcDir}/ImGuiImplGlfw.cpp
    ${srcDir}/ImGuiImplOpenGl3.cpp
)
