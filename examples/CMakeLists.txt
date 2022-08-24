function(imapp_add_test testName)
    add_executable(${PROJECT_NAME}_Example_${testName} ${testName}.cpp)

    target_link_libraries(${PROJECT_NAME}_Example_${testName} PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

    dp_configure_target(${PROJECT_NAME}_Example_${testName}
        DP_WARNINGS
        AUTO_SOURCE_GROUP
        WIN32_RELEASE
    )
endfunction()

imapp_add_test(HelloWorld)
imapp_add_test(LocaleExplorer)
imapp_add_test(MainWindowContent)
imapp_add_test(NoClose)
