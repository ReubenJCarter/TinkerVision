#include "MedianFilter.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class MedianFilter::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        int size; 
       
    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetThreshold(float t);
        void SetSize(int s);
};

std::map<ImageType, ComputeShader> MedianFilter::Internal::computeShaders;

std::string MedianFilter::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

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
        
    }
    
    imageStore(outputImage, id, d); 
}

)";

bool MedianFilter::Internal::shaderCompiled = false; 

MedianFilter::Internal::Internal()
{
    size = 7; 
}


void MedianFilter::Internal::Run(ImageGPU* input, ImageGPU* output)
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

    computeShader.SetInt("size", size);  

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void MedianFilter::Internal::Run(Image* input, Image* output)
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
            int inx = (i * input->GetWidth() + j);


        } 
    } 
}

void MedianFilter::Internal::SetSize(int s)
{
    size = s; 
}



MedianFilter::MedianFilter()
{
    internal = new Internal(); 
}

MedianFilter::~MedianFilter()
{
    delete internal; 
}

void MedianFilter::SetSize(int s)
{
    internal->SetSize(s);
}

void MedianFilter::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void MedianFilter::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}