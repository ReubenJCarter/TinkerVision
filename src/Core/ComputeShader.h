#pragma once

#include "ImageGPU.h"
#include "ShaderStorageBuffer.h"

#include <string>

namespace Viso
{
	
/**
 *An opengl compute shader class. 
 *The class handles compiling and passing uniform values, textures and buffers to the shader.
 */
class ComputeShader
{
	public: 
		/**An enum that describes the access profile for certain things passed to the compute shader*/
		enum Access{WRITE_ONLY, READ_ONLY, READ_WRITE}; 

	private:
		/**The openGL id of the compute shader*/
		unsigned int computeShader;
		/**The openGL id of the program object*/
		unsigned int computeProgram;
		
	public:
		/**The contstructor.*/
		ComputeShader();
		
		/**The destructor destroys all the opengl objects (the shader and program objects).*/
		~ComputeShader();

		/**Compiles the shader source code to make a compute shader.
		 *@param shaderSrc The source code to compile.
		*/
		void Compile(std::string shaderSrc);

		/**Run the compute shader with a number of work groups. This function is non blocking. 
		 *@param workGroupsX The number of work groups to run in the x dimension.
		 *@param workGroupsY The number of work groups to run in the y dimension.
		 *@param workGroupsZ The number of work groups to run in the z dimension.
		*/
		void Dispatch(unsigned int workGroupsX, unsigned int workGroupsY, unsigned int workGroupsZ);

		/**Blocks the current thread until the compute shader has finished running after a dispatch. */
		void Block();
		void SetFloat(std::string name, float value);
		void SetFloat2(std::string name, float value[2]);
		void SetFloat3(std::string name, float value[3]);
		void SetFloat4(std::string name, float value[4]);
		void SetInt(std::string name, int value);
		void SetInt2(std::string name, int value[2]);
		void SetInt3(std::string name, int value[3]);
		void SetInt4(std::string name, int value[4]);
		void SetUint(std::string name, unsigned int value);
		void SetUint2(std::string name, unsigned int value[2]);
		void SetUint3(std::string name, unsigned int value[3]);
		void SetUint4(std::string name, unsigned int value[4]);
		void SetMat2(std::string name, float value[4]);
		void SetMat3(std::string name, float value[9]);
		void SetMat4(std::string name, float value[16]);
		void SetImage(std::string name, ImageGPU* image, Access access=READ_WRITE);
		void SetImage(std::string name, ImageGPU* image, int imageUnit, Access access=READ_WRITE);
		void SetShaderStorageBuffer(std::string name, ShaderStorageBuffer* shaderStorageBuffer); 
};
}