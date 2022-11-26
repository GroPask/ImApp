function(imapp_add_example exampleName)
    add_executable(${PROJECT_NAME}_Example_${exampleName} ${exampleName}.cpp)
    target_link_libraries(${PROJECT_NAME}_Example_${exampleName} PRIVATE ${PROJECT_NAME}::${PROJECT_NAME})

    dp_configure_target(${PROJECT_NAME}_Example_${exampleName}
        DP_WARNINGS
        AUTO_SOURCE_GROUP
        WIN32_RELEASE
    )
endfunction()

imapp_add_example(DockSpaceMainWindow)
imapp_add_example(HelloWorld)
imapp_add_example(LocaleExplorer)
imapp_add_example(MainWindowContent)
imapp_add_example(NoClose)
