#include "GaussianDerivative.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

//https://slideplayer.com/slide/13588937/
class GaussianDerivative::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShadersHorizontal; 
        static std::map<ImageType, ComputeShader> computeShadersVertical; 
        static std::string shaderHorizontalSrc; 
        static std::string shaderVerticalSrc; 
        static bool shaderCompiled; 

        std::vector<float> guassFunc; 
        std::vector<float> guassFuncDeriv; 
        bool guassFuncGPUDirty; 
        ImageGPU guassFuncGPU; 
        ImageGPU guassFuncDerivGPU;

        ImageGPU workingImage; 

        float sigma; 
        int size; 
        GaussianDerivative::Direction direction; 
       
        inline float guass(float x, float sigma)
        {
            const static float PI = 3.14159265359;
            return 1.0f / (sqrt(2.0f * PI) * sigma) * exp( -((x*x) / (2 * sigma * sigma)) ); 
        }

        inline float dguass(float x, float sigma)
        {
            return (-x / (sigma * sigma)) * guass(x, sigma); 
        }

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetSigma(float s);
        void SetDirection(Direction dir);
};

std::map<ImageType, ComputeShader> GaussianDerivative::Internal::computeShadersHorizontal;
std::map<ImageType, ComputeShader> GaussianDerivative::Internal::computeShadersVertical;

std::string GaussianDerivative::Internal::shaderHorizontalSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (r32f, binding=2) readonly uniform image2D guassFunc; 

uniform float sigma; 
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 0);

    for(int i = -size+1; i < size; i++ )
    {
        vec4 g = imageLoad(guassFunc, ivec2(i + (size-1), 0)); 
        vec4 px = imageLoad(inputImage, id + ivec2(i, 0));
        d += px * vec4(g.r, g.r, g.r, g.r);
    }
    
    imageStore(outputImage, id, d); 
}

)";

std::string GaussianDerivative::Internal::shaderVerticalSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (r32f, binding=2) readonly uniform image2D guassFunc; 

uniform float sigma; 
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 1);

    for(int i = -size+1; i < size; i++ )
    {
        vec4 g = imageLoad(guassFunc, ivec2(i + (size-1), 0)); 
        vec4 px = imageLoad(inputImage, id + ivec2(0, i));
        d += px * vec4(g.r, g.r, g.r, g.r);
    }
    
    imageStore(outputImage, id, d); 
}

)";

bool GaussianDerivative::Internal::shaderCompiled = false; 

GaussianDerivative::Internal::Internal()
{
    SetSigma(1.6);  
}


void GaussianDerivative::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShadersHorizontal, shaderHorizontalSrc);
        CompileImageComputeShaders(computeShadersVertical, shaderVerticalSrc);
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }

    if(sigma == 0)
        return; 

    if(guassFuncGPUDirty)
    {
        guassFuncGPU.Allocate(guassFunc.size(), 1, ImageType::GRAYSCALE32F); 
        guassFuncGPU.Copy(&guassFunc[0], guassFunc.size(), 1); 

        guassFuncDerivGPU.Allocate(guassFuncDeriv.size(), 1, ImageType::GRAYSCALE32F); 
        guassFuncDerivGPU.Copy(&guassFuncDeriv[0], guassFuncDeriv.size(), 1); 

        guassFuncGPUDirty = false; 
    }

    workingImage.Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    
    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    //horizontal 
    ComputeShader& computeShaderHorizontal = computeShadersHorizontal[inputType];

    computeShaderHorizontal.SetFloat("sigma", sigma);  
    computeShaderHorizontal.SetInt("size", size); 

    if(direction == Direction::HORIZONTAL)
        computeShaderHorizontal.SetImage("guassFunc", &guassFuncDerivGPU, ComputeShader::READ_ONLY);
    else
        computeShaderHorizontal.SetImage("guassFunc", &guassFuncGPU, ComputeShader::READ_ONLY);

    computeShaderHorizontal.SetImage("inputImage", input);
    computeShaderHorizontal.SetImage("outputImage", &workingImage, ComputeShader::WRITE_ONLY);

    computeShaderHorizontal.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShaderHorizontal.Block();


    //vertical
    ComputeShader& computeShaderVertical = computeShadersVertical[inputType];

    computeShaderVertical.SetFloat("sigma", sigma);
    computeShaderVertical.SetInt("size", size);

    if(direction == Direction::HORIZONTAL)
        computeShaderHorizontal.SetImage("guassFunc", &guassFuncGPU, ComputeShader::READ_ONLY);
    else
        computeShaderHorizontal.SetImage("guassFunc", &guassFuncDerivGPU, ComputeShader::READ_ONLY);

    computeShaderVertical.SetImage("inputImage", &workingImage);
    computeShaderVertical.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShaderVertical.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShaderVertical.Block();
}

void GaussianDerivative::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void GaussianDerivative::Internal::SetSigma(float s)
{
    sigma = s; 

    if(sigma == 0)
        return; 

    size = ceil(3.0 * sigma); 

    //Compute Guass 1d
    guassFunc.clear(); 
    guassFunc.reserve(size * 2 - 1);
    for(int i = -size+1; i < size; i++)
    {
        float x = i;
        float v = guass(x, sigma); 
        guassFunc.push_back(v); 
    }

    //compute the 2d kernel by multipling the 1d together
    std::vector<float> gaussFuncFull;
    gaussFuncFull.reserve(guassFunc.size() * guassFunc.size());
    for(int i = 0; i < guassFunc.size(); i++)
    {
        for(int j = 0; j < guassFunc.size(); j++)
        {
            gaussFuncFull.push_back(guassFunc[i] * guassFunc[j]); 
        }
    }

    //Find the sum of the 2d kernel  
    float sum = 0;
    for(int i = 0; i < gaussFuncFull.size(); i++)
    {
        sum += gaussFuncFull[i];
    }  

    //normalise the 1d kernel
    for(int i = 0; i < guassFunc.size(); i++)
    {
        guassFunc[i] = guassFunc[i] / sum; 
    }



    //compute guass deriv
    guassFuncDeriv.clear();  
    guassFuncDeriv.reserve(size * 2 - 1);
    for(int i = -size+1; i < size; i++)
    {
        float x = i;
        float v = dguass(x, sigma);
        guassFuncDeriv.push_back(v);
    }
    std::vector<float> guassFuncDerivFull;
    guassFuncDerivFull.reserve(guassFuncDeriv.size() * guassFuncDeriv.size());
    for(int i = 0; i < guassFuncDeriv.size(); i++)
    {
        for(int j = 0; j < guassFuncDeriv.size(); j++)
        {
            guassFuncDerivFull.push_back(guassFuncDeriv[i] * guassFuncDeriv[j]); 
        }
    }
    float sumDeriv = 0;
    for(int i = 0; i < guassFuncDerivFull.size(); i++)
    {
        sumDeriv += guassFuncDerivFull[i];
    }  
    for(int i = 0; i < guassFuncDeriv.size(); i++)
    {
        guassFuncDeriv[i] = guassFuncDeriv[i] / sumDeriv; 
    }
    
    guassFuncGPUDirty = true; 
}

void GaussianDerivative::Internal::SetDirection(Direction dir)
{
    direction = dir; 
}



GaussianDerivative::GaussianDerivative()
{
    internal = new Internal(); 
}

GaussianDerivative::~GaussianDerivative()
{
    delete internal; 
}

void GaussianDerivative::SetSigma(float s)
{
    internal->SetSigma(s);
}

void GaussianDerivative::SetDirection(Direction dir)
{
    internal->SetDirection(dir); 
}

void GaussianDerivative::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void GaussianDerivative::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}