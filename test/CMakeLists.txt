add_executable(CoreSystemTest CoreSystemTest.cpp)
target_link_libraries(CoreSystemTest ${CMAKE_PROJECT_NAME})
install(TARGETS CoreSystemTest RUNTIME DESTINATION bin)
