#include "ImageGPU.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Visi 
{
	
ImageGPU::ImageGPU()
{
    width = 0;
    height = 0;
    type = ImageType::RGBA8;
}

ImageGPU::~ImageGPU()
{
    Deallocate();
}

void ImageGPU::Allocate(unsigned int w, unsigned int h, ImageType t)
{
    if(width != 0 || height !=0)
    {
        Deallocate();
    }

    width = w;
	height = h;
	type = t;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if(type == GRAYSCALE8)
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, width, height);
    }
    else if(type == GRAYSCALE16)
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16, width, height);
    }
    else if(type == GRAYSCALE32F)
    {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, width, height);
    }
    else if(type == RGB8)
    {
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
    }
    else if(type == RGB32F)
    {
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F, width, height);
    }
	else if(type == RGBA8)
    {
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    }
    else if(type == RGBA32F)
    {
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    }
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageGPU::Deallocate()
{
    glDeleteTextures(1, &texture);
    width = 0;
    height = 0; 
    type = ImageType::RGBA8;
}

unsigned int ImageGPU::GetWidth()
{
    return width;
}

unsigned int ImageGPU::GetHeight()
{
    return height;
}

ImageType ImageGPU::GetType()
{
    return type; 
}

void ImageGPU::Copy(void* data, int w, int h, int offX, int offY)
{
    int wfinal = w;
	int hfinal = h;
	if(w < 0 || h < 0)
	{
		wfinal = width;
		hfinal = height;
	}

    glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_RGBA, GL_FLOAT, data);
}

unsigned int ImageGPU::GetTexture()
{
    return texture; 
}
	
}