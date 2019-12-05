add_executable(CoreSystemTest CoreSystemTest.cpp)
target_link_libraries(CoreSystemTest ${CMAKE_PROJECT_NAME})
install(TARGETS CoreSystemTest RUNTIME DESTINATION bin)

add_executable(MarkerDetectionTest MarkerDetectionTest.cpp)
target_link_libraries(MarkerDetectionTest ${CMAKE_PROJECT_NAME})
install(TARGETS MarkerDetectionTest RUNTIME DESTINATION bin)