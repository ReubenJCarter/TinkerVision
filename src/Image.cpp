#include "Image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    if(width != 0 || height !=0)
    {
        Deallocate();
    }

    width = w;
	height = h;
	type = t;
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

unsigned int Image::GetWidth()
{
    return width;
}

unsigned int Image::GetHeight()
{
    return height;
}

ImageType Image::GetType()
{
    return type; 
}

unsigned char* Image::GetData()
{
    return data; 
}
	
}