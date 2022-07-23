add_executable(${PROJECT_NAME}_Test_Simple Simple.cpp)

target_link_libraries(${PROJECT_NAME}_Test_Simple PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

dp_configure_target(${PROJECT_NAME}_Test_Simple
    DP_WARNINGS
    AUTO_SOURCE_GROUP
    VS_STARTUP_PROJECT
    WIN32_RELEASE
)
