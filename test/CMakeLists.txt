#main Tests
add_executable(CoreSystemTest CoreSystemTest.cpp)
target_link_libraries(CoreSystemTest ${CMAKE_PROJECT_NAME})
install(TARGETS CoreSystemTest RUNTIME DESTINATION bin)

#Java Test
if(CREATE_JAVA_SUPPORT)
    add_jar(JavaTest INCLUDE_JARS ${CMAKE_PROJECT_NAME}_JAVAPACKAGE SOURCES javaTest.java) 
    install_jar(JavaTest DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()