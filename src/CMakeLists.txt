set(LIBRARYSRCFILES
	Core/BoundingBox.cpp
	Core/ComputeShader.cpp
	Core/Context.cpp
	Core/Contour.cpp
	Core/Image.cpp
	Core/ImageGPU.cpp
	Core/ParallelFor.cpp
	Core/ProcessHelper.cpp
	Core/ShaderStorageBuffer.cpp
	Core/Window.cpp

	IO/ReadImageFile.cpp
	IO/VideoFile.cpp
	IO/VideoHelper.cpp
	IO/WriteImageFile.cpp

	Process/AdaptiveThreshold.cpp
	Process/ApproxDistanceTransform.cpp
	Process/AverageFilter.cpp
	Process/BilateralFilter.cpp
	Process/Blend.cpp
	Process/BrightnessContrast.cpp
	Process/CameraDistortion.cpp
	Process/CannyEdgeDetect.cpp
	Process/ChannelDemux.cpp
	Process/ChannelMapper.cpp
	Process/ClearColor.cpp
	Process/CopyImage.cpp
	Process/CornerDetector.cpp
	Process/DownSample.cpp
	Process/FindContours.cpp
	Process/GaussianBlur.cpp
	Process/GaussianDerivative.cpp
	Process/GrayScale.cpp
	Process/HighLowThreshold.cpp
	Process/HSVToRGB.cpp
	Process/HysteresisEdgeThreshold.cpp
	Process/InRange.cpp
	Process/Invert.cpp
	Process/LocalMaxima.cpp
	Process/MedianFilter.cpp
	Process/Morph.cpp
	Process/NonMaximumEdgeSuppression.cpp
	Process/Normalize.cpp
	Process/Perspective3Point.cpp
	Process/Renderer.cpp
	Process/RGBToHSV.cpp
	Process/Sobel.cpp
	Process/Thinning.cpp
	Process/Threshold.cpp
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
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		COMPONENT MainLibrary  
)