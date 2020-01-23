#pragma once

#include "Visi_export.h"

#include <map>

#include <glm/glm.hpp>

#include "Image.h"


namespace Visi
{

class ComputeShader; 
class Image;
enum ImageType;

void CompileImageComputeShaders(std::map<ImageType, ComputeShader>& comuteShaderMap, std::string shaderSrc);
glm::ivec2 ComputeWorkGroupCount(glm::ivec2 dim, glm::ivec2 groupSize);


inline glm::vec4 GetPixel(unsigned char* data, ImageType imageType, int width, int height, int x, int y)
{
    if(x < 0 || y < 0 || x >= width || y >= height)
        return glm::vec4(0, 0, 0, 1); 

    unsigned int inx = (y * width + x) ;
    glm::vec4 out;
    if(imageType == ImageType::GRAYSCALE8)
    {
        out.r = ((float)data[inx]) / 255.0f; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE16)
    {
        out.r = ((float)((uint16_t*)data)[inx]) / 65535.0f; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE32F)
    {
        out.r = ((float*)data)[inx]; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::RGB8)
    {
        inx = inx * 3;
        out.r = ((float)data[inx + 2]) / 255.0f; 
        out.g = ((float)data[inx + 1]) / 255.0f; 
        out.b = ((float)data[inx + 0]) / 255.0f; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGB32F)
    {
        inx = inx * 3;
        out.r = ((float*)data)[inx + 2]; 
        out.g = ((float*)data)[inx + 1]; 
        out.b = ((float*)data)[inx + 0]; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGBA8)
    {
        inx = inx * 4;
        out.r = ((float)data[inx + 2]) / 255.0f; 
        out.g = ((float)data[inx + 1]) / 255.0f; 
        out.b = ((float)data[inx + 0]) / 255.0f; 
        out.a = ((float)data[inx + 3]) / 255.0f; 
    }
    else if(imageType == ImageType::RGBA32F)
    {
        inx = inx * 4;
        out.r = ((float*)data)[inx + 2]; 
        out.g = ((float*)data)[inx + 1]; 
        out.b = ((float*)data)[inx + 0]; 
        out.a = ((float*)data)[inx + 3]; 
    }
    return out;
}

inline glm::vec4 GetPixel(Image* image, int x, int y)
{
    unsigned char* data = image->GetData(); 
    ImageType imageType = image->GetType();
    return GetPixel(data, imageType, image->GetWidth(), image->GetHeight(), x, y); 
}

inline glm::ivec4 GetPixelUI(Image* image, int x, int y)
{
    if(x < 0 || y < 0 || x >= image->GetWidth() || y >= image->GetHeight())
        return glm::ivec4(0, 0, 0, 1); 

    
    unsigned char* data = image->GetData(); 
    ImageType imageType = image->GetType(); 
    unsigned int inx; 
    glm::ivec4 out;
    if(imageType == ImageType::GRAYSCALE8)
    {
        inx = (y * image->GetWidth() + x);
        out.r = (int)(data[inx]);
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE16)
    {
        inx = (y * image->GetWidth() + x);
        out.r = (int)((uint16_t*)data)[inx]; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE32F)
    {
        inx = (y * image->GetWidth() + x);
        out.r = ((float*)data)[inx] * 255; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::RGB8)
    {
        inx = (y * image->GetWidth() + x) * 3;
        out.r = (int)(data[inx + 2]); 
        out.g = (int)(data[inx + 1]); 
        out.b = (int)(data[inx + 0]); 
        out.a = 1;
    }
    else if(imageType == ImageType::RGB32F)
    {
        inx = (y * image->GetWidth() + x) * 3;
        out.r = ((float*)data)[inx + 2] * 255; 
        out.g = ((float*)data)[inx + 1] * 255; 
        out.b = ((float*)data)[inx + 0] * 255; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGBA8)
    {
        inx = (y * image->GetWidth() + x) * 4;
        out.r = (int)(data[inx + 2]); 
        out.g = (int)(data[inx + 1]); 
        out.b = (int)(data[inx + 0]); 
        out.a = (int)(data[inx + 3]); 
    }
    else if(imageType == ImageType::RGBA32F)
    {
        inx = (y * image->GetWidth() + x) * 4;
        out.r = ((float*)data)[inx + 2] * 255; 
        out.g = ((float*)data)[inx + 1] * 255; 
        out.b = ((float*)data)[inx + 0] * 255; 
        out.a = ((float*)data)[inx + 3] * 255; 
    }
    return out; 
}

inline glm::ivec4 GetPixelI(Image* image, int x, int y)
{
    if(x < 0 || y < 0 || x >= image->GetWidth() || y >= image->GetHeight())
        return glm::ivec4(0, 0, 0, 1); 

    
    unsigned char* data = image->GetData(); 
    ImageType imageType = image->GetType(); 
    unsigned int inx; 
    glm::ivec4 out;
    if(imageType == ImageType::GRAYSCALE8)
    {
        inx = (y * image->GetWidth() + x);
        out.r = (int)(data[inx]) - 128;
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE16)
    {
        inx = (y * image->GetWidth() + x);
        out.r = (int)((uint16_t*)data)[inx] - 32768; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE32F)
    {
        inx = (y * image->GetWidth() + x);
        out.r = ((float*)data)[inx] * 255; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::RGB8)
    {
        inx = (y * image->GetWidth() + x) * 3;
        out.r = (int)(data[inx + 2]) - 128; 
        out.g = (int)(data[inx + 1]) - 128; 
        out.b = (int)(data[inx + 0]) - 128; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGB32F)
    {
        inx = (y * image->GetWidth() + x) * 3;
        out.r = ((float*)data)[inx + 2] * 255; 
        out.g = ((float*)data)[inx + 1] * 255; 
        out.b = ((float*)data)[inx + 0] * 255; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGBA8)
    {
        inx = (y * image->GetWidth() + x) * 4;
        out.r = (int)(data[inx + 2]) - 128; 
        out.g = (int)(data[inx + 1]) - 128; 
        out.b = (int)(data[inx + 0]) - 128; 
        out.a = (int)(data[inx + 3]) - 128; 
    }
    else if(imageType == ImageType::RGBA32F)
    {
        inx = (y * image->GetWidth() + x) * 4;
        out.r = ((float*)data)[inx + 2] * 255; 
        out.g = ((float*)data)[inx + 1] * 255; 
        out.b = ((float*)data)[inx + 0] * 255; 
        out.a = ((float*)data)[inx + 3] * 255; 
    }
    return out; 
}

inline void SetPixel(unsigned char* data, ImageType imageType, int width, int height, int x, int y, glm::vec4 v)
{
    if(x < 0 || y < 0 || x >= width || y >= height)
        return; 

    unsigned int inx = (y * width + x); 
    if(imageType == ImageType::GRAYSCALE8)
    {
        data[inx] = v.r * 255;
    }
    else if(imageType == ImageType::GRAYSCALE16)
    {
        ((uint16_t*)data)[inx] = v.r * 65535;
    }
    else if(imageType == ImageType::GRAYSCALE32F)
    {
        ((float*)data)[inx] = v.r;
    }
    else if(imageType == ImageType::RGB8)
    {
        inx = inx * 3;
        data[inx + 2] = v.r * 255;
        data[inx + 1] = v.g * 255;
        data[inx + 0] = v.b * 255;
    }
    else if(imageType == ImageType::RGB32F)
    {
        inx = inx * 3;
        ((float*)data)[inx + 2]  = v.r;
        ((float*)data)[inx + 1]  = v.g;
        ((float*)data)[inx + 0]  = v.b;
    }
    else if(imageType == ImageType::RGBA8)
    {
        inx = inx * 4;
        data[inx + 2] = v.r * 255;
        data[inx + 1] = v.g * 255;
        data[inx + 0] = v.b * 255;
        data[inx + 3] = v.a * 255;
    }
    else if(imageType == ImageType::RGBA32F)
    {
        inx = inx * 4;
        ((float*)data)[inx + 2]  = v.r;
        ((float*)data)[inx + 1]  = v.g;
        ((float*)data)[inx + 0]  = v.b;
        ((float*)data)[inx + 3]  = v.a;
    }
}

inline void SetPixel(Image* image, int x, int y, glm::vec4 v)
{
    unsigned char* data = image->GetData(); 
    ImageType imageType = image->GetType(); 
    SetPixel(data, imageType, image->GetWidth(), image->GetHeight(), x, y, v); 
}

inline void SetPixelUI(Image* image, int x, int y, glm::ivec4 v)
{
    if(x < 0 || y < 0 || x >= image->GetWidth() || y >= image->GetHeight())
        return; 

    unsigned char* data = image->GetData(); 
    ImageType imageType = image->GetType(); 
    unsigned int inx; 
    if(imageType == ImageType::GRAYSCALE8)
    {
        inx = (y * image->GetWidth() + x) * 1;
        data[inx] = v.r;
    }
    else if(imageType == ImageType::GRAYSCALE16)
    {
        inx = (y * image->GetWidth() + x) * 1;
        ((uint16_t*)data)[inx] = v.r;
    }
    else if(imageType == ImageType::GRAYSCALE32F)
    {
        inx = (y * image->GetWidth() + x);
        ((float*)data)[inx] = v.r / 255.0f;
    }
    else if(imageType == ImageType::RGB8)
    {
        inx = (y * image->GetWidth() + x) * 3;
        data[inx + 2] = v.r;
        data[inx + 1] = v.g;
        data[inx + 0] = v.b;
    }
    else if(imageType == ImageType::RGB32F)
    {
        inx = (y * image->GetWidth() + x) * 3;
        ((float*)data)[inx + 2]  = v.r / 255.0f;
        ((float*)data)[inx + 1]  = v.g / 255.0f;
        ((float*)data)[inx + 0]  = v.b / 255.0f;
    }
    else if(imageType == ImageType::RGBA8)
    {
        inx = (y * image->GetWidth() + x) * 4;
        data[inx + 2] = v.r;
        data[inx + 1] = v.g;
        data[inx + 0] = v.b;
        data[inx + 3] = v.a;
    }
    else if(imageType == ImageType::RGBA32F)
    {
        inx = (y * image->GetWidth() + x) * 4;
        ((float*)data)[inx + 2]  = v.r / 255.0f;
        ((float*)data)[inx + 1]  = v.g / 255.0f;
        ((float*)data)[inx + 0]  = v.b / 255.0f;
        ((float*)data)[inx + 3]  = v.a / 255.0f;
    }
}


}