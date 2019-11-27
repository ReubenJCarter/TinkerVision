#include "ComputeShader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream> 
#include <vector>

namespace Visi 
{

ComputeShader::ComputeShader()
{
	
}

ComputeShader::~ComputeShader()
{
	glDeleteProgram(computeProgram);
	glDeleteShader(computeShader);
}

void ComputeShader::Compile(std::string shaderSrc)
{
	//create, compile shader src
	const char* src[1];
	src[0] = shaderSrc.c_str();
	computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, src, NULL);
	glCompileShader(computeShader);
	
	//check shader compilation for errors
	GLint isCompiled = 0;
	glGetShaderiv(computeShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(computeShader, maxLength, &maxLength, &errorLog[0]);

		//print error log
		std::cerr << &errorLog[0] << std::endl << std::endl;
		
		// Exit with failure.
		glDeleteShader(computeShader); // Don't leak the shader.
		return;
	}

	//create, link shader program
	computeProgram = glCreateProgram();
	glAttachShader(computeProgram, computeShader);
	glLinkProgram(computeProgram);
	
	//check shader link for errors
	GLint isLinked = 0;
	glGetProgramiv(computeProgram, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(computeProgram, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(computeProgram, maxLength, &maxLength, &infoLog[0]);
		
		//We don't need the program anymore.
		glDeleteProgram(computeProgram);
		//Don't leak shaders either.
		glDeleteShader(computeShader);

		//log the info
		std::cerr << &infoLog[0] << std::endl;
		
		//return;
	}

}

void ComputeShader::Dispatch(unsigned int workGroupsX, unsigned int workGroupsY, unsigned int workGroupsZ)
{
	glUseProgram(computeProgram);
	glDispatchCompute(workGroupsX, workGroupsY, workGroupsZ);
}

void ComputeShader::Block()
{
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

void ComputeShader::SetFloat(std::string name, float value)    {glProgramUniform1f(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value);}
void ComputeShader::SetFloat2(std::string name, float value[2]){glProgramUniform2f(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1]);}
void ComputeShader::SetFloat3(std::string name, float value[3]){glProgramUniform3f(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1], value[2]);}
void ComputeShader::SetFloat4(std::string name, float value[4]){glProgramUniform4f(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1], value[2], value[3]);}

void ComputeShader::SetInt(std::string name, int value)   {glProgramUniform1i(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value);}
void ComputeShader::SetInt2(std::string name, int value[2]){glProgramUniform2i(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1]);}
void ComputeShader::SetInt3(std::string name, int value[3]){glProgramUniform3i(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1], value[2]);}
void ComputeShader::SetInt4(std::string name, int value[4]){glProgramUniform4i(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1], value[2], value[3]);}

void ComputeShader::SetUint(std::string name, unsigned int value)   {glProgramUniform1ui(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value);}
void ComputeShader::SetUint2(std::string name, unsigned int value[2]){glProgramUniform2ui(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1]);}
void ComputeShader::SetUint3(std::string name, unsigned int value[3]){glProgramUniform3ui(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1], value[2]);}
void ComputeShader::SetUint4(std::string name, unsigned int value[4]){glProgramUniform4ui(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), value[0], value[1], value[2], value[3]);}
 
void ComputeShader::SetMat2(std::string name, float value[4]) {glProgramUniformMatrix2fv(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), 1, false, value);}
void ComputeShader::SetMat3(std::string name, float value[9]) {glProgramUniformMatrix3fv(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), 1, false, value);}
void ComputeShader::SetMat4(std::string name, float value[16]){glProgramUniformMatrix4fv(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), 1, false, value);}
 
void ComputeShader::SetImage(std::string name, ImageGPU* image, Access access)
{
	int imageUnit;
	glGetUniformiv(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), &imageUnit);
	
	ImageType type = image->GetType(); 

	int a = GL_READ_WRITE; 
	if(access == Access::WRITE_ONLY)
	{
		a = GL_WRITE_ONLY;
	}
	else if(access == Access::READ_ONLY)
	{
		a = GL_READ_ONLY;
	}
	
	if(type == GRAYSCALE8)
    {
        glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_R8);
    }
    else if(type == GRAYSCALE16)
    {
        glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_R16);
    }
    else if(type == GRAYSCALE32F)
    {
        glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_R32F);
    }
    else if(type == RGB8)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGB8);
    }
    else if(type == RGB32F)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGB32F);
    }
	else if(type == RGBA8)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGBA8);
    }
    else if(type == RGBA32F)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGBA32F);
    }
}

void ComputeShader::SetImage(std::string name, ImageGPU* image, int imageUnit, Access access)
{
	ImageType type = image->GetType(); 

	int a = GL_READ_WRITE; 
	if(access == Access::WRITE_ONLY)
	{
		a = GL_WRITE_ONLY;
	}
	else if(access == Access::READ_ONLY)
	{
		a = GL_READ_ONLY;
	}

	
	if(type == GRAYSCALE8)
    {
        glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_R8);
    }
    else if(type == GRAYSCALE16)
    {
        glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_R16);
    }
    else if(type == GRAYSCALE32F)
    {
        glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_R32F);
    }
    else if(type == RGB8)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGB8);
    }
    else if(type == RGB32F)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGB32F);
    }
	else if(type == RGBA8)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGBA8);
    }
    else if(type == RGBA32F)
    {
		glBindImageTexture(imageUnit, image->GetTexture(), 0, GL_FALSE, 0, a, GL_RGBA32F);
    }
}
/*
void ComputeShader::SetTexture(std::string name, Texture& texture, int textureUnit)
{
	glProgramUniform1i(computeProgram, glGetUniformLocation(computeProgram, name.c_str()), textureUnit);
	Texture::TYPE type = texture.GetType();
	if(type == Texture::TEXTURE_2D)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
	}
	else if(type == Texture::CUBE_MAP)
	{
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture.GetTexture());
	}
}
*/
}