#include "ImageGPU.h"

#include "Image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    if(width == w && height == h && type == t)
    {
        return; 
    }

    Deallocate();

    
    width = w;
	height = h;
	type = t;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

    if(w == 0 || h == 0)
    {
        std::cerr << "Visi:ImageGPU:Allocate:w=0||h=0\n"; 
        return; 
    }

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
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    }
    else if(type == RGB32F)
    {
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
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
    if(width != 0 && height != 0)
    {
        glDeleteTextures(1, &texture);
        width = 0;
        height = 0; 
        type = ImageType::RGBA8;
    }
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

unsigned int ImageGPU::GetTexture()
{
    return texture; 
}

void ImageGPU::Copy(void* data, int w, int h, int offX, int offY)
{
    int wfinal = w;
	int hfinal = h;

    if(type == GRAYSCALE8)
    {
        glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_RED, GL_UNSIGNED_BYTE, data);
    }
    else if(type == GRAYSCALE16)
    {
        glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_RED, GL_UNSIGNED_SHORT, data);
    }
    else if(type == GRAYSCALE32F)
    {
        glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_RED, GL_FLOAT, data);
    }
    else if(type == RGB8)
    {
		glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_BGR, GL_UNSIGNED_BYTE, data);
    }
    else if(type == RGB32F)
    {
		glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_BGR, GL_FLOAT, data);
    }
	else if(type == RGBA8)
    {
		glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_BGRA, GL_UNSIGNED_BYTE, data);
    }
    else if(type == RGBA32F)
    {
		glTextureSubImage2D(texture, 0, offX, offY, wfinal, hfinal, GL_BGRA, GL_FLOAT, data);
    }
}

void ImageGPU::Copy(Image* image)
{
    unsigned int w = image->GetWidth(); 
    unsigned int h = image->GetHeight(); 
    ImageType t = image->GetType(); 

    if(!IsSameDimensions(image))
    {
        Allocate(w, h, t); 
    }
    Copy(image->GetData(), w, h, 0, 0); 
}

bool ImageGPU::IsSameDimensions(ImageGPU* image)
{
    return image->GetWidth() == width && image->GetHeight() == height && image->GetType() == type; 
}

bool ImageGPU::IsSameDimensions(Image* image)
{
    return image->GetWidth() == width && image->GetHeight() == height && image->GetType() == type; 
}
	
}