set(srcDir .)
set(incDir ../include/ImApp)

target_sources(${PROJECT_NAME} PRIVATE CMakeLists.txt
    ${srcDir}/Context.cpp
    ${srcDir}/Context.hpp
    ${srcDir}/ImApp.cpp
    ${incDir}/ImApp.hpp
    ${incDir}/ImApp.inl
    ${srcDir}/ImGuiImplGlfw.cpp
    ${srcDir}/ImGuiImplOpenGl3.cpp
    ${incDir}/Utils.hpp
    ${srcDir}/SystemSpecific.hpp
)

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        ${srcDir}/SystemSpecificWin32.cpp
    )
else ()
    target_sources(${PROJECT_NAME} PRIVATE
        ${srcDir}/SystemSpecificDefault.cpp
    )
endif ()
