#include "BrightnessContrast.h"

#include "ComputeShader.h"

#include <string>
#include <iostream>

namespace Visi
{

class BrightnessContrast::Internal
{
    private:
        static ComputeShader computeShader; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        float brightness;
        float contrast; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetBrightness(float b);
        void SetContrast(float c);
};

ComputeShader BrightnessContrast::Internal::computeShader ;

std::string BrightnessContrast::Internal::shaderSrc = R"(
#version 430

layout(rgba8, binding=0) writeonly uniform image2D outputImage;
layout(rgba8, binding=1) uniform image2D inputImage;

uniform float contrast; 
uniform float brightness;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id) * contrast + vec4(brightness, brightness, brightness, 0.0f); 
    //vec4 d = vec4(1, 1, 0, 1);
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
        computeShader.Compile(shaderSrc);
        shaderCompiled = true; 
    }

    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }

    computeShader.SetFloat("contrast", contrast); 
    computeShader.SetFloat("brightness", brightness); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    int targetWidth = input->GetWidth(); 
    int targetHeight = input->GetHeight(); 

    int groupSize = 16;

    int groupW = targetWidth / groupSize; 
    int groupH = targetHeight / groupSize; 

    if(targetWidth % groupSize != 0)
        groupW++;
    if(targetHeight % groupSize != 0)
        groupH++;   

    computeShader.Dispatch(groupW, groupH, 1); 
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