#include "Sobel.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

class Sobel::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
};

std::map<ImageType, ComputeShader> Sobel::Internal::computeShaders;

std::string Sobel::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(rgba32f, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    //GRAD X
    float dx = 0;

    dx += imageLoad(inputImage, id + ivec2(-1, -1)).r * 1.0f;
    dx += imageLoad(inputImage, id + ivec2(-1,  0)).r * 2.0f;
    dx += imageLoad(inputImage, id + ivec2(-1,  1)).r * 1.0f;

    dx += imageLoad(inputImage, id + ivec2(1, -1)).r * -1.0f;
    dx += imageLoad(inputImage, id + ivec2(1,  0)).r * -2.0f;
    dx += imageLoad(inputImage, id + ivec2(1,  1)).r * -1.0f;

    //GRAD Y
    float dy = 0;

    dy += imageLoad(inputImage, id + ivec2(-1, -1)).r * 1.0f;
    dy += imageLoad(inputImage, id + ivec2( 0, -1)).r * 2.0f;
    dy += imageLoad(inputImage, id + ivec2( 1, -1)).r * 1.0f;

    dy += imageLoad(inputImage, id + ivec2(-1, 1)).r * -1.0f;
    dy += imageLoad(inputImage, id + ivec2(0,  1)).r * -2.0f;
    dy += imageLoad(inputImage, id + ivec2(1,  1)).r * -1.0f;

    float mag = length(vec2(dx, dy)); 
    float ori = atan(dy, dx); //equivelent of atan2 in glsl ...hmmm

    vec4 outVec = vec4(dx, dy, mag, ori); 

    imageStore(outputImage, id, outVec); 
}

)";

bool Sobel::Internal::shaderCompiled = false; 

Sobel::Internal::Internal()
{
}


void Sobel::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }
    
    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Sobel::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}




Sobel::Sobel()
{
    internal = new Internal(); 
}

Sobel::~Sobel()
{
    delete internal; 
}

void Sobel::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Sobel::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}