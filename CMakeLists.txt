cmake_minimum_required(VERSION 3.0.0)

project(Visi)
set(PROJECT_BRIEF ${CMAKE_PROJECT_NAME})


include (GenerateExportHeader)

set(SoftwareVersion 0.1)
add_definitions(-DSOFTWAREVERSION=${SoftwareVersion})

set(CMAKE_AUTOMOC ON)

set(CMAKE_BUILD_TYPE RELEASE)
set(GCC_COMPILE_FLAGS "-std=c++11 -Wall -O3 -g -static")
set(GCC_LINK_FLAGS "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")


#glm
set(GLM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm CACHE PATH "Path to GLM")
include_directories(${GLM_INC_DIR})

#ctpl
set(CTPL_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/ctpl CACHE PATH "Path to CTPL")
include_directories(${CTPL_INC_DIR})

#FreeImage
set(FREEIMAGE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/FreeImage/Dist/x64 CACHE PATH "Path to Free Image")
set(FREEIMAGE_INC_DIR ${FREEIMAGE_DIR})
set(FREEIMAGE_LIB_DIR ${FREEIMAGE_DIR})
set(FREEIMAGE_BIN_DIR ${FREEIMAGE_DIR})
include_directories(${FREEIMAGE_INC_DIR})
link_directories(${FREEIMAGE_LIB_DIR})

install(FILES ${FREEIMAGE_BIN_DIR}/FreeImage.dll
		DESTINATION bin
		COMPONENT MainLibrary
)

#GLFW
set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw-3.3/build/install CACHE PATH "Path to GLFW")
set(GLFW_INC_DIR ${GLFW_DIR}/include)
set(GLFW_LIB_DIR ${GLFW_DIR}/lib)
set(GLFW_BIN_DIR ${GLFW_DIR}/bin)
include_directories(${GLFW_INC_DIR})
link_directories(${GLFW_LIB_DIR})

install(FILES ${GLFW_BIN_DIR}/glfw3.dll
		DESTINATION bin
		COMPONENT MainLibrary
)

#GLEW
set(GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/glew-2.1.0 CACHE PATH "Path to GLEW")
set(GLEW_INC_DIR ${GLEW_DIR}/include)
set(GLEW_LIB_DIR ${GLEW_DIR}/lib/Release/x64)
set(GLEW_BIN_DIR ${GLEW_DIR}/bin/Release/x64)
include_directories(${GLEW_INC_DIR})
link_directories(${GLEW_LIB_DIR})

install(FILES ${GLEW_BIN_DIR}/glew32.dll
		DESTINATION bin
		COMPONENT MainLibrary
)

#cmake dirs
include_directories(${CMAKE_BINARY_DIR})
include_directories(src)
add_subdirectory(src)
add_subdirectory(test)

install(FILES ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_export.h
		DESTINATION include/${CMAKE_PROJECT_NAME}
)


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION include/${CMAKE_PROJECT_NAME}
		FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)