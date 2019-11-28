#include "BrightnessContrast.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class BrightnessContrast::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        float brightness;
        float contrast; 
       
    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetBrightness(float b);
        void SetContrast(float c);
};

std::map<ImageType, ComputeShader> BrightnessContrast::Internal::computeShaders;

std::string BrightnessContrast::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform float contrast; 
uniform float brightness;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id) * contrast + vec4(brightness, brightness, brightness, 0.0f); 
    imageStore(outputImage, id, d); 
}

)";

bool BrightnessContrast::Internal::shaderCompiled = false; 

BrightnessContrast::Internal::Internal()
{
    brightness = 0;
    contrast = 1; 
}


void BrightnessContrast::Internal::Run(ImageGPU* input, ImageGPU* output)
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

    computeShader.SetFloat("contrast", contrast); 
    computeShader.SetFloat("brightness", brightness); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void BrightnessContrast::Internal::Run(Image* input, Image* output)
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
            int inx = (i * input->GetWidth() + i);
        } 
    } 
}

void BrightnessContrast::Internal::SetBrightness(float b)
{
    brightness = b;
}

void BrightnessContrast::Internal::SetContrast(float c)
{
    contrast = c; 
}






BrightnessContrast::BrightnessContrast()
{
    internal = new Internal(); 
}

BrightnessContrast::~BrightnessContrast()
{
    delete internal; 
}

void BrightnessContrast::SetBrightness(float b)
{
    internal->SetBrightness(b);
}

void BrightnessContrast::SetContrast(float c)
{
    internal->SetContrast(c);
}

void BrightnessContrast::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void BrightnessContrast::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}