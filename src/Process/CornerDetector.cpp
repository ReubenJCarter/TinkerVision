#include "CornerDetector.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "GrayScale.h"
#include "GaussianBlur.h"
#include "Sobel.h"
#include "NonMaximumEdgeSuppression.h"


#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

//http://www.ipol.im/pub/art/2018/229/article_lr.pdf

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
        NonMaximumEdgeSuppression nms; 

        ImageGPU temp[4]; 

        float sigmaD; 
        float sigmaI; 
        float harrisK; 
        float shiTomasiThreshold;

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetSigmaI(float sig); 
        void SetSigmaD(float sig); 
        void SetK(float kk); 
        void SetHarrisK(float kk);
        void SetShiTomasiThreshold(float t);
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

uniform float harrisK;
uniform float shiTomasiThreshold;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id); 
    float IxIx = d.x;
    float IxIy = d.y;
    float IyIy = d.z;
    
    
    //compute the det and trace
    float detM = IxIx * IyIy - IxIy * IxIy;
    float traceM = IxIx + IyIy;
    float harrisResponse = detM - harrisK * traceM * traceM; 

    //Compute eigen values from det and trace
    //float lamd0 = (-traceM + sqrt(traceM * traceM - 4 * (-1) * (-detM) )) / (2 * -1);
    //float lamd1 = (-traceM - sqrt(traceM * traceM - 4 * (-1) * (-detM) )) / (2 * -1);
    //float shiTomasiResponse = min(lamd0, lamd1); 
    //shiTomasiResponse = shiTomasiResponse > shiTomasiThreshold ? shiTomasiResponse : 0; 
    float A = IxIx;
    float B = IxIy;
    float C = IyIy;
    float AsubC = (A - C); 
    float lamdaMin = (A + C - sqrt( AsubC*AsubC + 4 * B*B ) ) / 2;
    float shiTomasiResponse = lamdaMin > shiTomasiThreshold ? lamdaMin : 0;


    imageStore(outputImage, id, vec4(shiTomasiResponse, harrisResponse, 0, 1)); 
}

)";

bool CornerDetector::Internal::shaderCompiled = false; 

CornerDetector::Internal::Internal()
{
    sigmaI = 2; 
    sigmaD = 1; 
    harrisK = 0.05;
    shiTomasiThreshold = 0.001;
}


void CornerDetector::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(harrisShaders, harrisShaderSrc); 
        CompileImageComputeShaders(structureTensorShaders, structureTensorShaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    //Grayscale if needed ( color in, gray output)
    if( !(input->GetType() == ImageType::GRAYSCALE8 || input->GetType() == ImageType::GRAYSCALE16 || input->GetType() == ImageType::GRAYSCALE32F ) )
    {
        grayscale.Run(input, &temp[0] ); 
        input = &temp[0]; 
    } 

    //Gaussian Blur ( gray in, gray output)
    gaussianBlur.SetSigma(sigmaD); 
    gaussianBlur.Run(input, &temp[1]);

    //Sobel Gradients ( gray in, 4xfloat output)
    sobel.Run(&temp[1], &temp[2]); 

    //Compute Ix^2 IxIy Iy^2
    if( temp[3].GetWidth() != input->GetWidth() || temp[3].GetHeight() != input->GetHeight() || temp[3].GetType() != ImageType::RGBA32F)
    {
        temp[3].Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F);
    }
    ComputeShader& structureTensorShader = structureTensorShaders[ImageType::RGBA32F];
    structureTensorShader.SetImage("inputImage", &temp[2]);
    structureTensorShader.SetImage("outputImage", &temp[3], ComputeShader::WRITE_ONLY);
    structureTensorShader.Dispatch(groupCount.x, groupCount.y, 1); 
    structureTensorShader.Block(); 

    //Gaussian Blur ( gray in, gray output)
    gaussianBlur.SetSigma(sigmaI); 
    gaussianBlur.Run(&temp[3], &temp[2]);

    //Compute Harris response
    ComputeShader& harrisShader = harrisShaders[ImageType::RGBA32F];
    harrisShader.SetFloat("harrisK", harrisK); 
    harrisShader.SetFloat("shiTomasiThreshold", shiTomasiThreshold);
    harrisShader.SetImage("inputImage", &temp[2]);
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

void CornerDetector::Internal::SetSigmaI(float sig)
{
    sigmaI = sig; 
}

void CornerDetector::Internal::SetSigmaD(float sig)
{
    sigmaD = sig; 
}

void CornerDetector::Internal::SetHarrisK(float kk)
{
    harrisK = kk;
}

void CornerDetector::Internal::SetShiTomasiThreshold(float t)
{
    shiTomasiThreshold = t;
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