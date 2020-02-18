set(LIBRARYSRCFILES
	AdaptiveThreshold.cpp
	ApproxDistanceTransform.cpp
	BilateralFilter.cpp
	Blend.cpp
	BoundingBox.cpp
	BrightnessContrast.cpp
	CameraDistortion.cpp
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
	HighLowThreshold.cpp
	HSVToRGB.cpp
	HysteresisEdgeThreshold.cpp
	Image.cpp
	ImageGPU.cpp
	InRange.cpp
	Invert.cpp
	LocalMaxima.cpp
	MedianFilter.cpp
	Morph.cpp
	NonMaximumSuppression.cpp
	Normalize.cpp
	ParallelFor.cpp
	Perspective3Point.cpp
	ProcessHelper.cpp
	ReadImageFile.cpp
	Renderer.cpp
	RGBToHSV.cpp
	ShaderStorageBuffer.cpp
	Sobel.cpp
	Thinning.cpp
	Threshold.cpp
	VideoFile.cpp
	VideoHelper.cpp
	Window.cpp
	WriteImageFile.cpp
)

add_library(${CMAKE_PROJECT_NAME} SHARED ${LIBRARYSRCFILES})
generate_export_header(${CMAKE_PROJECT_NAME} EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_export.h)
target_link_libraries(${CMAKE_PROJECT_NAME} FreeImage)
target_link_libraries(${CMAKE_PROJECT_NAME} glew32)
target_link_libraries(${CMAKE_PROJECT_NAME} opengl32)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw3dll)

if (USE_FFMPEG)
	target_link_libraries(${CMAKE_PROJECT_NAME} avcodec avfilter avformat avutil postproc swresample swscale)
endif() 

install(TARGETS ${CMAKE_PROJECT_NAME} 
		ARCHIVE DESTINATION lib
		RUNTIME DESTINATION bin
		COMPONENT MainLibrary  
)