#include "GaussianBlur.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
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

        float sigma; 
        int size; 
       
        inline float guass(float x, float sigma)
        {
            const static float PI = 3.14159265359;
            return 1.0f / (sqrt(2.0f * PI) * sigma) * exp( -(x*x / (2 * sigma * sigma)) ); 
        }

    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetThreshold(float t);
        void SetSigma(float s);
};

std::map<ImageType, ComputeShader> GaussianBlur::Internal::computeShaders;

std::string GaussianBlur::Internal::shaderHorizontalSrc = R"(

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

    vec4 orig = imageLoad(inputImage, id);

    for(int i = 0; i < size; i++ )
    {
        vec4 px = imageLoad(inputImage, id + ivec2(i - size/2 , 0));
    }
    
    imageStore(outputImage, id, d); 
}

)";

std::string GaussianBlur::Internal::shaderVerticalSrc = R"(

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

    vec4 orig = imageLoad(inputImage, id);

    for(int i = 0; i < size; i++ )
    {
        vec4 px = imageLoad(inputImage, id + ivec2(0 , i - size/2));
    }
    
    imageStore(outputImage, id, d); 
}

)";

bool GaussianBlur::Internal::shaderCompiled = false; 

GaussianBlur::Internal::Internal()
{
    guassFuncGPUDirty = true; 
    SetSigma(1.6);  
}


void GaussianBlur::Internal::Run(ImageGPU* input, ImageGPU* output)
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

    if(guassFuncGPUDirty)
    {
        guassFuncGPU.Allocate(size, 1, ImageType::GRAYSCALE32F); 
        guassFuncGPU.Copy(&guassFunc[0], size, 1); 
        guassFuncGPUDirty = false; 
    }
    
    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetFloat("sigma", sigma);  
    computeShader.SetInt("size", size); 
    computeShader.SetImage("guassFunc", &guassFuncGPU, ComputeShader::READ_ONLY);

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void GaussianBlur::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void GaussianBlur::Internal::SetSigma(float s)
{
    sigma = s; 
    size = ceil(3 * sigma); 

    guassFunc.clear(); 
    guassFunc.reserve(size);
     
    for(int i = 0;  i < size; i++)
    {
        float x = i;
        float v = guass(x, sigma); 
        guassFunc.push_back(v); 
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