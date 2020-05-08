#include "ShaderStorageBuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
 
namespace Viso
{

ShaderStorageBuffer::ShaderStorageBuffer()
{
	size = 0;
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	Deallocate(); 
}


void ShaderStorageBuffer::Allocate(unsigned int sz, unsigned int bindingP, unsigned char* data, USAGE u)
{
    if(sz == size)
        return; 

    if(sz == 0)
        return;

    Deallocate(); 

    unsigned int usageGL;

    if(u == USAGE::DYNAMIC)
    {
        usageGL = GL_DYNAMIC_DRAW;
    }
    else if(u == USAGE::STREAM)
    {
        usageGL = GL_STREAM_DRAW;
    }
    else
    {
        usageGL = GL_STATIC_DRAW;
    }

	size = sz;
	glGenBuffers(1, &storageBuffer);
	bindingPoint = bindingP;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, storageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, storageBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::Deallocate()
{
    if(size > 0)
    {
        size = 0; 
	    glDeleteBuffers(1, &storageBuffer);
    }
}

void ShaderStorageBuffer::Copy(unsigned char* data, USAGE u)
{
    if(size == 0)
        return;

    unsigned int usageGL;

    if(u == USAGE::DYNAMIC)
    {
        usageGL = GL_DYNAMIC_DRAW;
    }
    else if(u == USAGE::STREAM)
    {
        usageGL = GL_STREAM_DRAW;
    }
    else
    {
        usageGL = GL_STATIC_DRAW;
    }

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, storageBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usageGL);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::Get(unsigned char* data)
{
    if(size == 0)
    {
        return;
    }
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, storageBuffer);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

unsigned int ShaderStorageBuffer::GetBindingPoint()
{
	return bindingPoint;
}

unsigned int ShaderStorageBuffer::Size()
{
	return size;
}

}