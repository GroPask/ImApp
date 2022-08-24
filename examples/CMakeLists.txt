add_executable(${PROJECT_NAME}_Example_LocaleExplorer LocaleExplorer.cpp)

target_link_libraries(${PROJECT_NAME}_Example_LocaleExplorer PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

dp_configure_target(${PROJECT_NAME}_Example_LocaleExplorer
    DP_WARNINGS
    AUTO_SOURCE_GROUP
    WIN32_RELEASE
)
