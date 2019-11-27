#pragma once

#include "ImageGPU.h"

#include <string>

namespace Visi
{
	
class ComputeShader
{
	private:
		unsigned int computeShader;
		unsigned int computeProgram;
		
	public:
		ComputeShader();
		~ComputeShader();
		void Compile(std::string shaderSrc);
		void Dispatch(unsigned int workGroupsX, unsigned int workGroupsY, unsigned int workGroupsZ);
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
		void SetImage(std::string name, ImageGPU* image);
		void SetImage(std::string name, ImageGPU* image, int imageUnit);
};
}