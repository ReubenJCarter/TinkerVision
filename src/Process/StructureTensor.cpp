#include "StructureTensor.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

class StructureTensor::Internal
{
    private:
        static std::map<ImageType, ComputeShader> structureTensorShaders; 
        static std::string structureTensorShaderSrc; 
        static bool shaderCompiled; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
};

std::map<ImageType, ComputeShader> StructureTensor::Internal::structureTensorShaders;
std::string StructureTensor::Internal::structureTensorShaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id); 
    
    //offset gradients
    d.x -= 0.5f;
    d.y -= 0.5f;

    vec4 outD = vec4(  d.x * d.x,   d.x * d.y,   d.y * d.y,   1);
    imageStore(outputImage, id, outD); 
}

)";

bool StructureTensor::Internal::shaderCompiled = false; 

StructureTensor::Internal::Internal()
{
}


void StructureTensor::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(structureTensorShaders, structureTensorShaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& structureTensorShader = structureTensorShaders[ImageType::RGBA32F];
    structureTensorShader.SetImage("inputImage", input);
    structureTensorShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);
    structureTensorShader.Dispatch(groupCount.x, groupCount.y, 1); 
    structureTensorShader.Block(); 
}

void StructureTensor::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::GRAYSCALE32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE32F); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}




StructureTensor::StructureTensor()
{
    internal = new Internal(); 
}

StructureTensor::~StructureTensor()
{
    delete internal; 
}

void StructureTensor::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void StructureTensor::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}