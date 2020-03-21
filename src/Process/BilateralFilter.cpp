#include "BilateralFilter.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class BilateralFilter::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        float sigma; 
        int size; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetSigma(float s);
        void SetSize(int s);
};

std::map<ImageType, ComputeShader> BilateralFilter::Internal::computeShaders;

//https://www.shadertoy.com/view/4dfGDH
std::string BilateralFilter::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform float sigma; 
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 1);

    imageStore(outputImage, id, d); 
}

)";

bool BilateralFilter::Internal::shaderCompiled = false; 

BilateralFilter::Internal::Internal()
{
    sigma = 0.5;
    size = 7; 
}


void BilateralFilter::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetFloat("sigma", sigma);
    computeShader.SetInt("size", size);  

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void BilateralFilter::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 

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

void BilateralFilter::Internal::SetSigma(float s)
{
    sigma = s;
}

void BilateralFilter::Internal::SetSize(int s)
{
    size = s; 
}



BilateralFilter::BilateralFilter()
{
    internal = new Internal(); 
}

BilateralFilter::~BilateralFilter()
{
    delete internal; 
}

void BilateralFilter::SetSigma(float s)
{
    internal->SetSigma(s);
}

void BilateralFilter::SetSize(int s)
{
    internal->SetSize(s);
}

void BilateralFilter::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void BilateralFilter::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}