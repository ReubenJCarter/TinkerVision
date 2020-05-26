set(LIBRARYSRCFILES

	Algo/FixedRadiusNN.cpp
	Algo/Perspective3Point.cpp

	CompositeProcess/ARUCODetector.cpp
	CompositeProcess/BarcodeDetector.cpp
	CompositeProcess/CannyEdgeDetect.cpp
	CompositeProcess/CornerDetector.cpp
	CompositeProcess/Resize.cpp
	CompositeProcess/Sift.cpp

	ComputeGraph/Graph.cpp
	ComputeGraph/Node.cpp
	ComputeGraph/RegisterNodes.cpp

	Core/BoundingBox.cpp
	Core/ComputeShader.cpp
	Core/Context.cpp
	Core/Contour.cpp
	Core/Image.cpp
	Core/ImageGPU.cpp
	Core/ParallelFor.cpp
	Core/SerializedObject.cpp
	Core/ProcessHelper.cpp
	Core/ShaderStorageBuffer.cpp
	Core/Window.cpp

	IO/ImageFile.cpp
	IO/VideoFile.cpp
	IO/VideoHelper.cpp

	Process/AdaptiveThreshold.cpp
	Process/ApproxDistanceTransform.cpp
	Process/AverageFilter.cpp
	Process/BilateralFilter.cpp
	Process/Blend.cpp
	Process/BrightnessContrast.cpp
	Process/CameraDistortion.cpp
	Process/ChannelDemux.cpp
	Process/ChannelMapper.cpp
	Process/ChannelMux.cpp
	Process/CircleHoughTransform.cpp
	Process/ClearColor.cpp
	Process/Convolution.cpp
	Process/CopyImage.cpp
	Process/Crop.cpp
	Process/Downsample.cpp
	Process/FindContours.cpp
	Process/GaussianBlur.cpp
	Process/GaussianDerivative.cpp
	Process/GaussianFunction.cpp
	Process/GrayScale.cpp
	Process/HarrisShiTomasiResponse.cpp
	Process/HighLowThreshold.cpp
	Process/HSVToRGB.cpp
	Process/HysteresisEdgeThreshold.cpp
	Process/InRange.cpp
	Process/Invert.cpp
	Process/LocalMaxima.cpp
	Process/MarkerBitExtract.cpp
	Process/MarkerDictionary.cpp
	Process/MedianFilter.cpp
	Process/Mipmaps.cpp
	Process/MorphRect.cpp
	Process/MorphThinning.cpp
	Process/NonMaximumEdgeSuppression.cpp
	Process/Normalize.cpp
	Process/OtsuThreshold.cpp
	Process/Renderer.cpp
	Process/RGBToHSV.cpp
	Process/Rotate.cpp
	Process/Sobel.cpp
	Process/StereoMatchSAD.cpp
	Process/StructureTensor.cpp
	Process/TemplateMatch.cpp
	Process/Threshold.cpp
	Process/Translate.cpp
	Process/Upsample.cpp
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

#Build CMD tool
set(CMDToolSrc 
	CMDTool.cpp 
)

add_executable(${CMAKE_PROJECT_NAME}_CMD ${CMDToolSrc})
target_link_libraries(${CMAKE_PROJECT_NAME}_CMD ${CMAKE_PROJECT_NAME})

install(TARGETS ${CMAKE_PROJECT_NAME}_CMD
		RUNTIME DESTINATION bin
		COMPONENT MainLibrary  
)


#java language support
if(CREATE_JAVA_SUPPORT)
	SET_SOURCE_FILES_PROPERTIES(SwigInterface.i PROPERTIES CPLUSPLUS ON)
	#set(CMAKE_SWIG_FLAGS -package ${CMAKE_PROJECT_NAME})	
	#SET_SOURCE_FILES_PROPERTIES(SwigInterface.i PROPERTIES SWIG_FLAGS "-includeall")
	SET_SOURCE_FILES_PROPERTIES(SwigInterface.i PROPERTIES SWIG_FLAGS "-package \"TnkrVisJava\"")
	SWIG_ADD_LIBRARY(${CMAKE_PROJECT_NAME}_Java TYPE SHARED LANGUAGE java OUTPUT_DIR java/ SOURCES SwigInterface.i )
	SWIG_LINK_LIBRARIES(${CMAKE_PROJECT_NAME}_Java ${CMAKE_PROJECT_NAME} )
	install(TARGETS ${CMAKE_PROJECT_NAME}_Java 
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib
		RUNTIME DESTINATION bin
		COMPONENT MainLibrary  
	)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/java/ DESTINATION java/ FILES_MATCHING PATTERN "*.java")
	 
	file(GLOB_RECURSE JAVA_BINDING_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/java/*.java )
	#message( ${JAVA_BINDING_SRC} )
	add_jar(${CMAKE_PROJECT_NAME}_JAVAPACKAGE SOURCES ${JAVA_BINDING_SRC})
    install_jar(${CMAKE_PROJECT_NAME}_JAVAPACKAGE DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endif()


#Building the GUI 
if(BUILD_GUI)
	set(GUISRCFILES
		
		${Q_DARK_STYLE_SHEET_QRC_FILE}
		
		GUI/BuildEditor/BuildEditorWidget.cpp

		GUI/NodeEditor/BaseNode.cpp
		GUI/NodeEditor/NodeEditorWidget.cpp

		GUI/ProjectHierarchy/ProjectHierarchyWidget.cpp
		GUI/ProjectHierarchy/HierarchyListModel.cpp
		GUI/ProjectHierarchy/AddGraphDialog.cpp
		GUI/ProjectHierarchy/DeleteGraphDialog.cpp
		GUI/ProjectHierarchy/YesNoDialog.cpp

		GUI/Main.cpp
		GUI/MainWindow.cpp
	)

	add_executable(${CMAKE_PROJECT_NAME}_GUI ${GUISRCFILES})
	target_link_libraries(${CMAKE_PROJECT_NAME}_GUI Qt5::Core Qt5::Widgets Qt5::OpenGL)
	target_link_libraries(${CMAKE_PROJECT_NAME}_GUI debug Debug/nodes optimized Release/nodes)
	target_link_libraries(${CMAKE_PROJECT_NAME}_GUI ${CMAKE_PROJECT_NAME})
	install(TARGETS ${CMAKE_PROJECT_NAME}_GUI RUNTIME DESTINATION bin)
endif()