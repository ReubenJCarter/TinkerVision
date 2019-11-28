#include "ProcessHelper.h"
#include "ComputeShader.h"
#include "ImageType.h"

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

}