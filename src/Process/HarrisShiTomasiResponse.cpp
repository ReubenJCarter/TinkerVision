#include "HarrisShiTomasiResponse.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Process/GrayScale.h"
#include "../Process/GaussianBlur.h"
#include "../Process/Sobel.h"
#include "../Process/NonMaximumEdgeSuppression.h"


#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Viso
{
namespace Process
{

//http://www.ipol.im/pub/art/2018/229/article_lr.pdf

class HarrisShiTomasiResponse::Internal
{
    private:
        static std::map<ImageType, ComputeShader> harrisShaders; 
        static std::string harrisShaderSrc; 
        static bool shaderCompiled; 

        float harrisK; 
        float shiTomasiThreshold;

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetHarrisK(float kk);
        void SetShiTomasiThreshold(float t);
};

std::map<ImageType, ComputeShader> HarrisShiTomasiResponse::Internal::harrisShaders;
std::string HarrisShiTomasiResponse::Internal::harrisShaderSrc = R"(

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

bool HarrisShiTomasiResponse::Internal::shaderCompiled = false; 

HarrisShiTomasiResponse::Internal::Internal()
{
    harrisK = 0.05;
    shiTomasiThreshold = 0.001;
}


void HarrisShiTomasiResponse::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(harrisShaders, harrisShaderSrc); 
        shaderCompiled = true; 
    }
   
    ReallocateIfNotSameSize(output, input, ImageType::RGBA32F); 

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    //Compute Harris response
    ComputeShader& harrisShader = harrisShaders[ImageType::RGBA32F];
    harrisShader.SetFloat("harrisK", harrisK); 
    harrisShader.SetFloat("shiTomasiThreshold", shiTomasiThreshold);
    harrisShader.SetImage("inputImage", input);
    harrisShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);
    harrisShader.Dispatch(groupCount.x, groupCount.y, 1); 
    harrisShader.Block();

}

void HarrisShiTomasiResponse::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input, ImageType::RGBA32F); 
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void HarrisShiTomasiResponse::Internal::SetHarrisK(float kk)
{
    harrisK = kk;
}

void HarrisShiTomasiResponse::Internal::SetShiTomasiThreshold(float t)
{
    shiTomasiThreshold = t;
}




HarrisShiTomasiResponse::HarrisShiTomasiResponse()
{
    internal = new Internal(); 
}

HarrisShiTomasiResponse::~HarrisShiTomasiResponse()
{
    delete internal; 
}

void HarrisShiTomasiResponse::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void HarrisShiTomasiResponse::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}