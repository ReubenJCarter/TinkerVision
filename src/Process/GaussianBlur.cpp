#include "GaussianBlur.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace TnkrVis
{
namespace Process
{

class GaussianBlur::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShadersHorizontal; 
        static std::map<ImageType, ComputeShader> computeShadersVertical; 
        static std::string shaderHorizontalSrc; 
        static std::string shaderVerticalSrc; 
        static bool shaderCompiled; 

        std::vector<float> guassFunc; 
        bool guassFuncGPUDirty; 
        ImageGPU guassFuncGPU; 

        ImageGPU workingImage; 

        float sigma; 
        int size; 
       
        inline float guass(float x, float sigma)
        {
            const static float PI = 3.14159265359;
            return 1.0f / (sqrt(2.0f * PI) * sigma) * exp( -((x*x) / (2 * sigma * sigma)) ); 
        }

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetSigma(float s);
};

std::map<ImageType, ComputeShader> GaussianBlur::Internal::computeShadersHorizontal;
std::map<ImageType, ComputeShader> GaussianBlur::Internal::computeShadersVertical;

std::string GaussianBlur::Internal::shaderHorizontalSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (r32f, binding=2) readonly uniform image2D guassFunc; 

uniform float sigma; 
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 0);

    for(int i = 0; i < size; i++ )
    {
        vec4 g = imageLoad(guassFunc, ivec2(i, 0)); 
        vec4 px = imageLoad(inputImage, id + ivec2(i, 0));
        d += px * vec4(g.r, g.r, g.r, g.r);
    }

    for(int i = 1; i < size; i++)
    {
        vec4 g = imageLoad(guassFunc, ivec2(i, 0)); 
        vec4 px = imageLoad(inputImage, id - ivec2(i, 0));
        d += px * vec4(g.r, g.r, g.r, g.r);
    }
    
    imageStore(outputImage, id, d); 
}

)";

std::string GaussianBlur::Internal::shaderVerticalSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (r32f, binding=2) readonly uniform image2D guassFunc; 

uniform float sigma; 
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 1);

    for(int i = 0; i < size; i++ )
    {
        vec4 g = imageLoad(guassFunc, ivec2(i, 0)); 
        vec4 px = imageLoad(inputImage, id + ivec2(0, i));
        d += px * vec4(g.r, g.r, g.r, g.r);
    }

    for(int i = 1; i < size; i++)
    {
        vec4 g = imageLoad(guassFunc, ivec2(i, 0)); 
        vec4 px = imageLoad(inputImage, id - ivec2(0, i));
        d += px * vec4(g.r, g.r, g.r, g.r);
    }
    
    imageStore(outputImage, id, d); 
}

)";

bool GaussianBlur::Internal::shaderCompiled = false; 

GaussianBlur::Internal::Internal()
{
    SetSigma(1.6);  
}


void GaussianBlur::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShadersHorizontal, shaderHorizontalSrc); 
        CompileImageComputeShaders(computeShadersVertical, shaderVerticalSrc); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    if(sigma == 0)
        return; 

    if(guassFuncGPUDirty)
    {
        guassFuncGPU.Allocate(size, 1, ImageType::GRAYSCALE32F); 
        guassFuncGPU.Copy(&guassFunc[0], size, 1); 
        guassFuncGPUDirty = false; 
    }

    workingImage.Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    
    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShaderHorizontal = computeShadersHorizontal[inputType];

    computeShaderHorizontal.SetFloat("sigma", sigma);  
    computeShaderHorizontal.SetInt("size", size); 
    computeShaderHorizontal.SetImage("guassFunc", &guassFuncGPU, ComputeShader::READ_ONLY);

    computeShaderHorizontal.SetImage("inputImage", input);
    computeShaderHorizontal.SetImage("outputImage", &workingImage, ComputeShader::WRITE_ONLY);

    computeShaderHorizontal.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShaderHorizontal.Block();

    ComputeShader& computeShaderVertical = computeShadersVertical[inputType];

    computeShaderVertical.SetFloat("sigma", sigma);
    computeShaderVertical.SetInt("size", size);
    computeShaderVertical.SetImage("guassFunc", &guassFuncGPU, ComputeShader::READ_ONLY);

    computeShaderVertical.SetImage("inputImage", &workingImage);
    computeShaderVertical.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShaderVertical.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShaderVertical.Block();
}

void GaussianBlur::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void GaussianBlur::Internal::SetSigma(float s)
{
    sigma = s; 

    if(sigma == 0)
        return; 

    size = ceil(3.0 * sigma); 

    //Compute Guass 1d
    guassFunc.clear(); 
    guassFunc.reserve(size);

    for(int i = 0; i < size; i++)
    {
        float x = i;
        float v = guass(x, sigma); 
        guassFunc.push_back(v); 
    }

    //compute the 2d kernel by multipling the 1d together
    std::vector<float> gaussFuncFull;
    gaussFuncFull.reserve(size * size);
    for(int i = -size+1; i < size; i++)
    {
        for(int j = -size+1; j < size; j++)
        {
            gaussFuncFull.push_back(guassFunc[abs(i)] * guassFunc[abs(j)]); 
        }
    }

    //Find the sum of the 2d kernel  
    float sum = 0;
    for(int i = 0; i < gaussFuncFull.size(); i++)
    {
        sum += gaussFuncFull[i];
    }  

    //normalise the 1d kernel
    for(int i = 0; i < size; i++)
    {
        guassFunc[i] = guassFunc[i] / sum; 
    }
    
    guassFuncGPUDirty = true; 
}



GaussianBlur::GaussianBlur()
{
    internal = new Internal(); 
}

GaussianBlur::~GaussianBlur()
{
    delete internal; 
}

void GaussianBlur::SetSigma(float s)
{
    internal->SetSigma(s);
}

void GaussianBlur::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void GaussianBlur::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}