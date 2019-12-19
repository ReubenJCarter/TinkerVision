set(LIBRARYSRCFILES
	AdaptiveThreshold.cpp
	BilateralFilter.cpp
	Blend.cpp
	BrightnessContrast.cpp
	CannyEdgeDetect.cpp
	ChannelDemux.cpp
	ClearColor.cpp
	ComputeShader.cpp
	Context.cpp
	Contour.cpp
	CopyImage.cpp
	CornerDetector.cpp
	FindContours.cpp
	GaussianBlur.cpp
	GaussianDerivative.cpp
	GrayScale.cpp
	HSVToRGB.cpp
	HysteresisEdgeThreshold.cpp
	Image.cpp
	ImageGPU.cpp
	Invert.cpp
	MedianFilter.cpp
	Morph.cpp
	NonMaximumSuppression.cpp
	Normalize.cpp
	ProcessHelper.cpp
	ReadImageFile.cpp
	Renderer.cpp
	RGBToHSV.cpp
	ShaderStorageBuffer.cpp
	Sobel.cpp
	SuzukiContour.cpp
	Threshold.cpp
	WriteImageFile.cpp
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