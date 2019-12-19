#include "CornerDetector.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include "GrayScale.h"
#include "GaussianBlur.h"
#include "Sobel.h"


#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

class CornerDetector::Internal
{
    private:
        static std::map<ImageType, ComputeShader> harrisShaders; 
        static std::string harrisShaderSrc; 
        static std::map<ImageType, ComputeShader> structureTensorShaders; 
        static std::string structureTensorShaderSrc; 
        static bool shaderCompiled; 

        GrayScale grayscale; 
        GaussianBlur gaussianBlur; 
        Sobel sobel; 

        ImageGPU temp[4]; 

        float sigma; 
        float k; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetSigma(float sig); 
        void SetK(float kk); 
};

std::map<ImageType, ComputeShader> CornerDetector::Internal::structureTensorShaders;
std::string CornerDetector::Internal::structureTensorShaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id); 
    vec4 outD = vec4(  d.x * d.x,   d.x * d.y,   d.y * d.y,   1);
    imageStore(outputImage, id, outD); 
}

)";

std::map<ImageType, ComputeShader> CornerDetector::Internal::harrisShaders;
std::string CornerDetector::Internal::harrisShaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform float k;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id); 
    
    float detA = d.x * d.z - d.y * d.y;

    float traceA = d.x + d.z;

    float harrisResponse = detA - k * traceA * traceA; 
    imageStore(outputImage, id, vec4(harrisResponse, harrisResponse, harrisResponse, 1)); 
}

)";

bool CornerDetector::Internal::shaderCompiled = false; 

CornerDetector::Internal::Internal()
{
    sigma = 1; 
    k = 0.05;
}


void CornerDetector::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(harrisShaders, harrisShaderSrc); 
        CompileImageComputeShaders(structureTensorShaders, structureTensorShaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE32F); 
    }

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    //Grayscale if needed ( color in, gray output)
    if( !(input->GetType() == ImageType::GRAYSCALE8 || input->GetType() == ImageType::GRAYSCALE16 || input->GetType() == ImageType::GRAYSCALE32F ) )
    {
        grayscale.Run(input, &temp[0] ); 
        input = &temp[0]; 
    } 

    //Sobel Gradients ( gray in, 4xfloat output)
    sobel.Run(input, &temp[1]); 

    //Compute Ix^2 IxIy Iy^2
    if( temp[2].GetWidth() != input->GetWidth() || temp[2].GetHeight() != input->GetHeight() || temp[2].GetType() != ImageType::RGBA32F)
    {
        temp[2].Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F);
    }
    ComputeShader& structureTensorShader = structureTensorShaders[ImageType::RGBA32F];
    structureTensorShader.SetImage("inputImage", &temp[1]);
    structureTensorShader.SetImage("outputImage", &temp[2], ComputeShader::WRITE_ONLY);
    structureTensorShader.Dispatch(groupCount.x, groupCount.y, 1); 
    structureTensorShader.Block();

    //Gaussian Blur ( gray in, gray output)
    gaussianBlur.SetSigma(sigma); 
    gaussianBlur.Run(&temp[2], &temp[1]); 

    //Compute Harris response
    ImageType inputType = input->GetType();
    ComputeShader& harrisShader = harrisShaders[inputType];
    harrisShader.SetFloat("k", k); 
    harrisShader.SetImage("inputImage", &temp[1]);
    harrisShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);
    harrisShader.Dispatch(groupCount.x, groupCount.y, 1); 
    harrisShader.Block();
    
}

void CornerDetector::Internal::Run(Image* input, Image* output)
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

void CornerDetector::Internal::SetSigma(float sig)
{
    sigma = sig; 
}

void CornerDetector::Internal::SetK(float kk)
{
    k = kk;
}




CornerDetector::CornerDetector()
{
    internal = new Internal(); 
}

CornerDetector::~CornerDetector()
{
    delete internal; 
}

void CornerDetector::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void CornerDetector::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}