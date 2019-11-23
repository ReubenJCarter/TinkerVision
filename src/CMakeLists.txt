set(LIBRARYSRCFILES ${CMAKE_PROJECT_NAME}_Src)
set(LIBRARYSRCFILES
	Context.cpp
	FileLoader.cpp
	FileSaver.cpp
	Image.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${LIBRARYSRCFILES})
generate_export_header(${CMAKE_PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_export.h)
target_link_libraries(${CMAKE_PROJECT_NAME} FreeImage)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw3dll)



install(TARGETS ${CMAKE_PROJECT_NAME} 
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		COMPONENT MainLibrary  
)