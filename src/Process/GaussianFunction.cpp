#include "GaussianFunction.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Viso
{
namespace Process
{

class GaussianFunction::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 
       
        float sigmaX; 
        float sigmaY; 
        GenerateMode genMode; 

    public:
        Internal(); 
        void Run(ImageGPU* dst);
        void Run(Image* dst);
        void SetSigma(float sig); 
        void SetSigma(float sigX, float sigY);
        void SetGenerateMode(GenerateMode gm); 
};

std::map<ImageType, ComputeShader> GaussianFunction::Internal::computeShaders;

std::string GaussianFunction::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D dstImage; 

uniform float sigma2X; 
uniform float sigma2Y;

uniform int halfW;
uniform int halfH; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    int xx = id.x - halfW; 
    int yy = id.y - halfH; 
    float av = exp( -(xx*xx/(2.0f*sigma2X) + yy*yy/(2.0f*sigma2Y)) ); 
    imageStore(dstImage, id, vec4(av, av, av, 1)); 
}

)";

bool GaussianFunction::Internal::shaderCompiled = false; 

GaussianFunction::Internal::Internal()
{
    sigmaX = 1.6f; 
    sigmaY = 1.6f; 
    genMode = GenerateMode::FIXED;  
}


void GaussianFunction::Internal::Run(ImageGPU* dst)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ImageType inputType = dst->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    float sigma2X = sigmaX*sigmaX; 
    float sigma2Y = sigmaY*sigmaY; 

    if(genMode == GenerateMode::SIZE_TO_SIGMA)
    {
        int dstW = ceil(sigmaX*4);
        int dstH = ceil(sigmaY*4);
        dstW = dstW%2==0 ? dstW+1 : dstW; 
        dstH = dstH%2==0 ? dstH+1 : dstH; 
        dst->Allocate( dstW, dstH, dst->GetType() ); 
    }
    else if(genMode == GenerateMode::SIGMA_TO_SIZE)
    {
        sigma2X = (float)dst->GetWidth()/4.0f;
        sigma2Y = (float)dst->GetHeight()/4.0f;
    }

    int halfW = dst->GetWidth()/2; 
    int halfH = dst->GetHeight()/2; 

    computeShader.SetInt("halfW", halfW); 
    computeShader.SetInt("halfH", halfH); 

    computeShader.SetFloat("sigma2X", sigma2X); 
    computeShader.SetFloat("sigma2Y", sigma2Y); 
    computeShader.SetImage("dstImage", dst, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(dst->GetWidth(), dst->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void GaussianFunction::Internal::Run(Image* dst)
{
    float sigma2X = sigmaX*sigmaX; 
    float sigma2Y = sigmaY*sigmaY; 

    if(genMode == GenerateMode::SIZE_TO_SIGMA)
    {
        int dstW = ceil(sigmaX*4);
        int dstH = ceil(sigmaY*4);
        dstW = dstW%2==0 ? dstW+1 : dstW; 
        dstH = dstH%2==0 ? dstH+1 : dstH; 
        dst->Allocate( dstW, dstH, dst->GetType() ); 
    }
    else if(genMode == GenerateMode::SIGMA_TO_SIZE)
    {
        sigma2X = (float)dst->GetWidth()/4.0f;
        sigma2Y = (float)dst->GetHeight()/4.0f;
    }

    int halfW = dst->GetWidth()/2; 
    int halfH = dst->GetHeight()/2; 

    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, dst, sigma2X, sigma2Y, halfW, halfH](int x, int y)
    {        
        glm::vec4 outVec ; 

        int xx = x - halfW; 
        int yy = y - halfH; 

        float av = exp( -(xx*xx/(2*sigma2X) + yy*yy/(2*sigma2Y)) );
        outVec.r = av;
        outVec.g = av;
        outVec.b = av;
        outVec.a = 1;

        SetPixel(dst, x, y, outVec);   
    };

    pf.Run(dst->GetWidth(), dst->GetHeight(), kernel); 
}

void GaussianFunction::Internal::SetSigma(float sig)
{
    sigmaX = sig; 
    sigmaY = sig; 
}

void GaussianFunction::Internal::SetSigma(float sigX, float sigY)
{
    sigmaX = sigX; 
    sigmaY = sigY; 
}

void GaussianFunction::Internal::SetGenerateMode(GenerateMode gm)
{
    genMode = gm; 
}



GaussianFunction::GaussianFunction()
{
    internal = new Internal(); 
}

GaussianFunction::~GaussianFunction()
{ 
    delete internal; 
}

void GaussianFunction::SetSigma(float sig)
{
    internal->SetSigma(sig); 
}

void GaussianFunction::SetSigma(float sigX, float sigY)
{
    internal->SetSigma(sigX, sigY); 
}

void GaussianFunction::SetGenerateMode(GenerateMode gm)
{
    internal->SetGenerateMode(gm); 
}

void GaussianFunction::Run(ImageGPU* dst)
{
    internal->Run(dst); 
}

void GaussianFunction::Run(Image* dst)
{
    internal->Run(dst); 
}


}
}