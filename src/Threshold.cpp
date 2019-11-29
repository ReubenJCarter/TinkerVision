#include "Threshold.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class Threshold::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        float threshold; 
       
    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetThreshold(float t);
};

std::map<ImageType, ComputeShader> Threshold::Internal::computeShaders;

std::string Threshold::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform float threshold; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    d.r = d.r < threshold ? 0.0f : 1.0f; 
    d.g = d.g < threshold ? 0.0f : 1.0f; 
    d.b = d.b < threshold ? 0.0f : 1.0f; 
    imageStore(outputImage, id, d); 
}

)";

bool Threshold::Internal::shaderCompiled = false; 

Threshold::Internal::Internal()
{
    threshold = 0.5;
}


void Threshold::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetFloat("threshold", threshold); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Threshold::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    for(int i = 0; i < input->GetHeight(); i++)
    {
        for(int j = 0; j < input->GetWidth(); j++)
        {
            int inx = (i * input->GetWidth() + j);


        } 
    } 
}

void Threshold::Internal::SetThreshold(float t)
{
    threshold = t;
}




Threshold::Threshold()
{
    internal = new Internal(); 
}

Threshold::~Threshold()
{
    delete internal; 
}

void Threshold::SetThreshold(float t)
{
    internal->SetThreshold(t);
}

void Threshold::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Threshold::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}