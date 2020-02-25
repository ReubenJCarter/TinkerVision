#include "AdaptiveThreshold.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class AdaptiveThreshold::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        float threshold; 
        int size; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetThreshold(float t);
        void SetSize(int s);
};

std::map<ImageType, ComputeShader> AdaptiveThreshold::Internal::computeShaders;

//http://homepages.inf.ed.ac.uk/rbf/HIPR2/adpthrsh.htm
std::string AdaptiveThreshold::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform float threshold; 
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 1);

    vec4 orig = imageLoad(inputImage, id);

    for(int j = 0; j < size; j++ )
    for(int i = 0; i < size; i++ )
    {

        vec4 px = imageLoad(inputImage, id + ivec2(i - size/2 , j - size/2));
        d.rgb += px.rgb; 
    }

    d.rgb /= size*size; 

    d.rgb -= orig.rgb;

    d.r = d.r < threshold ? 1.0f : 0.0f; 
    d.g = d.g < threshold ? 1.0f : 0.0f; 
    d.b = d.b < threshold ? 1.0f : 0.0f; 

    imageStore(outputImage, id, d); 
}

)";

bool AdaptiveThreshold::Internal::shaderCompiled = false; 

AdaptiveThreshold::Internal::Internal()
{
    threshold = 0.5;
    size = 7; 
}


void AdaptiveThreshold::Internal::Run(ImageGPU* input, ImageGPU* output)
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

    computeShader.SetFloat("threshold", threshold);
    computeShader.SetInt("size", size);  

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void AdaptiveThreshold::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 pix = GetPixel(input, x, y); 
        glm::vec4 d (0, 0, 0, 1);
        for(int j = 0; j < size; j++ )
        for(int i = 0; i < size; i++ )
        {

            glm::vec4 px = GetPixel(input, x + i - size/2, y + j - size/2);
            d.r += px.r; 
            d.g += px.g; 
            d.b += px.b; 
        }

        d.r /= size*size; 
        d.g /= size*size; 
        d.b /= size*size; 

        d.r -= pix.r;
        d.g -= pix.g;
        d.b -= pix.b;

        d.r = d.r < threshold ? 1.0f : 0.0f; 
        d.g = d.g < threshold ? 1.0f : 0.0f; 
        d.b = d.b < threshold ? 1.0f : 0.0f; 

        SetPixel(output, x, y, d); 

    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
    
}

void AdaptiveThreshold::Internal::SetThreshold(float t)
{
    threshold = t;
}

void AdaptiveThreshold::Internal::SetSize(int s)
{
    size = s; 
}



AdaptiveThreshold::AdaptiveThreshold()
{
    internal = new Internal(); 
}

AdaptiveThreshold::~AdaptiveThreshold()
{
    delete internal; 
}

void AdaptiveThreshold::SetThreshold(float t)
{
    internal->SetThreshold(t);
}

void AdaptiveThreshold::SetSize(int s)
{
    internal->SetSize(s);
}

void AdaptiveThreshold::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void AdaptiveThreshold::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}