#include "ImageGPU.h"

#include "Image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace TnkrVis 
{
	
ImageGPU::ImageGPU()
{
    width = 0;
    height = 0;
    type = ImageType::RGBA8;
}

ImageGPU::ImageGPU(const ImageGPU& im)
{
    width = 0; 
    height = 0; 
    type = im.type; 
}

ImageGPU::~ImageGPU()
{
    Deallocate();
}

void ImageGPU::AllocateLike(Image* referanceImage)
{
    Allocate(referanceImage->GetWidth(), referanceImage->GetHeight(), referanceImage->GetType());
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
        glBindTexture(GL_TEXTURE_2D, 0);
        std::cerr << "TnkrVis:ImageGPU:Allocate:w=0||h=0\n"; 
        return; 
    }

    //set gl pixel alignment, does this affect perfromance??
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

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
    if(width > 0 && height > 0)
    {
        glDeleteTextures(1, &texture);
        width = 0;
        height = 0; 
        type = ImageType::RGBA8;
    }
}

void ImageGPU::Copy(void* data, int w, int h, int offX, int offY)
{
    int wfinal = w;
	int hfinal = h;

    if(w == 0 || h == 0)
    {
        std::cerr << "TnkrVis:ImageGPU:Copy:w=0||h=0\n"; 
        return; 
    }

    //set gl pixel alignment, does this affect perfromance??
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    
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

bool ImageGPU::IsSameSize(ImageGPU* image)
{
    return image->GetWidth() == width && image->GetHeight() == height; 
}

bool ImageGPU::IsSameSize(Image* image)
{
    return image->GetWidth() == width && image->GetHeight() == height; 
}

bool ImageGPU::IsSameType(ImageGPU* image)
{
    return image->GetType() == type; 
}

bool ImageGPU::IsSameType(Image* image)
{
    return image->GetType() == type; 
}



int ImageGPU::GetChannelCount()
{
    if(type == GRAYSCALE8)
    {
        return 1; 
    }
    else if(type == GRAYSCALE16)
    {
        return 1; 
    }
    else if(type == GRAYSCALE32F)
    {
        return 1; 
    }
    else if(type == RGB8)
    {
        return 3; 
    }
    else if(type == RGB32F)
    {
        return 3; 
    }
	else if(type == RGBA8)
    {
        return 4; 
    }
    else if(type == RGBA32F)
    {
        return 4; 
    }
    return 0; 
}

	
}