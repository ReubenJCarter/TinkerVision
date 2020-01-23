#include "Image.h"

#include "ImageGPU.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Visi 
{
	
Image::Image()
{
    width = 0;
    height = 0;
    type = ImageType::RGBA8;
    data = NULL; 
}

Image::~Image()
{
    Deallocate();
}

void Image::Allocate(unsigned int w, unsigned int h, ImageType t)
{
    if(width == w && height == h && type == t)
    {
        return; 
    }

    Deallocate();

    width = w;
	height = h;
	type = t;

    if(w == 0 || h == 0)
        return; 

	if(type == GRAYSCALE8)
    {
        data = new unsigned char[width * height];
    }
    else if(type == GRAYSCALE16)
    {
        data = new unsigned char[width * height * 2];
    }
    else if(type == GRAYSCALE32F)
    {
        data = new unsigned char[width * height * 4]; 
    }
    else if(type == RGB8)
    {
        data = new unsigned char[width * height * 3]; 
    }
    else if(type == RGB32F)
    {
        data = new unsigned char[width * height * 12]; 
    }
	else if(type == RGBA8)
    {
        data = new unsigned char[width * height * 4]; 
    }
    else if(type == RGBA32F)
    {
        data = new unsigned char[width * height * 16]; 
    }
}

void Image::Deallocate()
{
    if(width != 0 || height !=0)
    {
        delete[] data;
    
        width = 0;
        height = 0; 
        type = ImageType::RGBA8;
    }
}

void Image::Copy(ImageGPU* image)
{
    if(!(image->GetWidth() > 0 && image->GetHeight() > 0))
    {
        return; 
    }
    if(!IsSameDimensions(image))
    {
        Allocate(image->GetWidth(), image->GetHeight(), image->GetType()); 
    }

    unsigned int texture = image->GetTexture();

    if(type == GRAYSCALE8)
    {
        //This seems to fail with some non standard image sizes. 
        glGetTextureImage(texture, 0, GL_RED, GL_UNSIGNED_BYTE, width * height, data);
    }
    else if(type == GRAYSCALE16)
    {
        glGetTextureImage(texture, 0, GL_RED, GL_UNSIGNED_SHORT, width * height * 2, data);
    }
    else if(type == GRAYSCALE32F)
    {
        glGetTextureImage(texture, 0, GL_RED, GL_FLOAT, width * height * 4, data);
    }
    else if(type == RGB8)
    {
        glGetTextureImage(texture, 0, GL_BGR, GL_UNSIGNED_BYTE, width * height * 3, data);
    }
    else if(type == RGB32F)
    {
        glGetTextureImage(texture, 0, GL_BGR, GL_FLOAT, width * height * 12, data);
    }
	else if(type == RGBA8)
    {
        glGetTextureImage(texture, 0, GL_BGRA, GL_UNSIGNED_BYTE, width * height * 4, data);
    }
    else if(type == RGBA32F)
    {
        glGetTextureImage(texture, 0, GL_BGRA, GL_FLOAT, width * height * 16, data);
    }
}

void Image::Copy(Image* image)
{
    if(!(image->GetWidth() > 0 && image->GetHeight() > 0))
    {
        return; 
    }
    if(!IsSameDimensions(image))
    {
        Allocate(image->GetWidth(), image->GetHeight(), image->GetType()); 
    }

    if(type == GRAYSCALE8)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight(); i++)
        {
            data[i] = image->data[i]; 
        }
    }
    else if(type == GRAYSCALE16)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight() * 2; i++)
        {
            data[i] = image->data[i]; 
        }
    }
    else if(type == GRAYSCALE32F)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight() * 4; i++)
        {
            data[i] = image->data[i]; 
        }
    }
    else if(type == RGB8)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight() * 3; i++)
        {
            data[i] = image->data[i]; 
        }
    }
    else if(type == RGB32F)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight() * 12; i++)
        {
            data[i] = image->data[i]; 
        }
    }
	else if(type == RGBA8)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight() * 4; i++)
        {
            data[i] = image->data[i]; 
        }
    }
    else if(type == RGBA32F)
    {
        for(unsigned int i = 0; i < image->GetWidth() * image->GetHeight() * 16; i++)
        {
            data[i] = image->data[i]; 
        }
    }
    
}

bool Image::IsSameDimensions(ImageGPU* image)
{
    return image->GetWidth() == width && image->GetHeight() == height && image->GetType() == type; 
}

bool Image::IsSameDimensions(Image* image)
{
    return image->GetWidth() == width && image->GetHeight() == height && image->GetType() == type; 
}
	
}