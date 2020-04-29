#include "CopyImage.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>
#include <algorithm>

namespace Visi
{
namespace Process
{

class CopyImage::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled;    
        
        bool formatTranslate; 
        bool useOutputSize; 
        glm::ivec2 offset; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetFormatTranslate(bool ftr);
        void UseOutputSize(bool outSz){useOutputSize = outSz; }
        void SetOffset(int x, int y){offset = glm::ivec2(x, y); }; 
};

std::map<ImageType, ComputeShader> CopyImage::Internal::computeShaders;

std::string CopyImage::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage; //Output here does not need a format qualifier because its write only
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform ivec2 offset; 
uniform ivec2 targetMin; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id + targetMin - offset);
    imageStore(outputImage, id + targetMin, d); 
}

)";

bool CopyImage::Internal::shaderCompiled = false; 

CopyImage::Internal::Internal()
{
    formatTranslate = true; 
    useOutputSize = false; 
    offset = glm::ivec2(0, 0); 
}


void CopyImage::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }


    if(!useOutputSize)
    {
        if(formatTranslate)
        {
            ReallocateIfNotSameSize(output, input); 
        }
        else
        {
            ReallocateSame(output, input); 
        }
    }

    ImageType inputType = input->GetType();

    glm::ivec2 targetMin;
    glm::ivec2 targetMax;
    targetMin.x = (std::max)(offset.x, 0);
    targetMin.y = (std::max)(offset.y, 0);
    targetMax.x = (std::min)(offset.x + input->GetWidth(), output->GetWidth() );
    targetMax.y = (std::min)(offset.y + input->GetHeight(), output->GetHeight() );

    targetMax.x = (std::max)(targetMax.x, 0); 
    targetMax.y = (std::max)(targetMax.y, 0); 

    if(targetMax.x - targetMin.x <= 0 || targetMax.y -  targetMin.y <= 0)
        return; 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetInt2("targetMin", glm::value_ptr(targetMin)); 
    computeShader.SetInt2("offset", glm::value_ptr(offset)); 
    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(targetMax.x - targetMin.x, targetMax.y -  targetMin.y), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void CopyImage::Internal::Run(Image* input, Image* output)
{
    if(formatTranslate)
    {
        ReallocateIfNotSameSize(output, input,  output->GetType()); 
    }
    else
    {
        ReallocateSame(output, input); 
    }
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            SetPixel(output, i, j, GetPixel(input, i, j)); 

        } 
    } 
}

void CopyImage::Internal::SetFormatTranslate(bool ftr)
{
    formatTranslate = ftr;
}



CopyImage::CopyImage()
{
    internal = new Internal(); 
}

CopyImage::~CopyImage()
{
    delete internal; 
}

void CopyImage::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void CopyImage::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void CopyImage::SetFormatTranslate(bool ftr)
{
    internal->SetFormatTranslate(ftr);
}

void CopyImage::UseOutputSize(bool outSz)
{
    internal->UseOutputSize(outSz);
}

void CopyImage::SetOffset(int x, int y)
{
    internal->SetOffset(x, y); 
}

}
}