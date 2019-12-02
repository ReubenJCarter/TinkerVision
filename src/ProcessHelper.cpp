#include "ProcessHelper.h"
#include "ComputeShader.h"
#include "ImageType.h"
#include "Image.h"

namespace Visi
{

void CompileImageComputeShaders(std::map<ImageType, ComputeShader>& computeShaderMap, std::string shaderSrc)
{
    std::string glslVersion = "#version 430\n"; 
    std::string finalSrc;
    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER r8\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::GRAYSCALE8].Compile(finalSrc); 

    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER r16\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::GRAYSCALE16].Compile(finalSrc); 

    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER r32f\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::GRAYSCALE32F].Compile(finalSrc); 

    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER rgba8\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::RGB8].Compile(finalSrc); 

    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER rgba32f\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::RGB32F].Compile(finalSrc);

    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER rgba8\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::RGBA8].Compile(finalSrc); 
    
    finalSrc = glslVersion;
    finalSrc += "#define FORMAT_QUALIFIER rgba32f\n";
    finalSrc += shaderSrc;
    computeShaderMap[ImageType::RGBA32F].Compile(finalSrc); 
}

glm::ivec2 ComputeWorkGroupCount(glm::ivec2 dim, glm::ivec2 groupSize)
{
    int targetWidth = dim.x;
    int targetHeight = dim.y;

    int groupW = targetWidth / groupSize.x; 
    int groupH = targetHeight / groupSize.y; 

    if(targetWidth % groupSize.x != 0)
        groupW++;
    if(targetHeight % groupSize.y != 0)
        groupH++;   

    return glm::ivec2(groupW, groupH); 
}

glm::vec4 GetPixel(Image* image, int x, int y)
{
    if(x < 0 || y < 0 || x >= image->GetWidth() || y >= image->GetHeight())
        return glm::vec4(0, 0, 0, 1); 

    
    unsigned char* data = image->GetData(); 
    ImageType imageType = image->GetType(); 
    unsigned int inx; 
    glm::vec4 out;
    if(imageType == ImageType::GRAYSCALE8)
    {
        inx = (y * image->GetWidth() + x) * 1;
        out.r = ((float)data[inx]) / 255.0f; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE16)
    {
        inx = (y * image->GetWidth() + x) * 2;
        out.r = ((float)((uint16_t*)data)[inx]) / 255.0f; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::GRAYSCALE32F)
    {
        inx = (y * image->GetWidth() + x);
        out.r = ((float*)data)[inx]; 
        out.g = 0;
        out.b = 0;
        out.a = 1;
    }
    else if(imageType == ImageType::RGB8)
    {
        inx = (y * image->GetWidth() + x) * 3;
        out.r = ((float)data[inx + 2]) / 255.0f; 
        out.g = ((float)data[inx + 1]) / 255.0f; 
        out.b = ((float)data[inx + 0]) / 255.0f; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGB32F)
    {
        inx = (y * image->GetWidth() + x) * 3;
        out.r = ((float*)data)[inx + 2]; 
        out.g = ((float*)data)[inx + 1]; 
        out.b = ((float*)data)[inx + 0]; 
        out.a = 1;
    }
    else if(imageType == ImageType::RGBA8)
    {
        inx = (y * image->GetWidth() + x) * 4;
        out.r = ((float)data[inx + 2]) / 255.0f; 
        out.g = ((float)data[inx + 1]) / 255.0f; 
        out.b = ((float)data[inx + 0]) / 255.0f; 
        out.a = ((float)data[inx + 3]) / 255.0f; 
    }
    else if(imageType == ImageType::RGBA32F)
    {
        inx = (y * image->GetWidth() + x) * 4;
        out.r = ((float*)data)[inx + 2]; 
        out.g = ((float*)data)[inx + 1]; 
        out.b = ((float*)data)[inx + 0]; 
        out.a = ((float*)data)[inx + 3]; 
    }
    return out;
}

glm::ivec4 GetPixelUI(Image* image, int x, int y)
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

glm::ivec4 GetPixelI(Image* image, int x, int y)
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

}