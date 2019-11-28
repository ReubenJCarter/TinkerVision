set(LIBRARYSRCFILES
	BrightnessContrast.cpp
	ComputeShader.cpp
	Context.cpp
	Image.cpp
	ImageGPU.cpp
	ProcessHelper.cpp
	ReadImageFile.cpp
	WriteImageFile.cpp
	Sobel.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${LIBRARYSRCFILES})
generate_export_header(${CMAKE_PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_export.h)
target_link_libraries(${CMAKE_PROJECT_NAME} FreeImage)
target_link_libraries(${CMAKE_PROJECT_NAME} opengl32)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw3dll)
target_link_libraries(${CMAKE_PROJECT_NAME} glew32)


install(TARGETS ${CMAKE_PROJECT_NAME} 
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		COMPONENT MainLibrary  
)