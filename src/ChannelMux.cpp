#include "ChannelMux.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class ChannelMux::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        int channel;
       
    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* input,ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetChannel(int ch); 
};

std::map<ImageType, ComputeShader> ChannelMux::Internal::computeShaders;

std::string ChannelMux::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform int channel;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);

    if(channel == 0)
    {
        d = glm::vec4(d.r, 0, 0, 1);
    }
    else if(channel == 1)
    {
        d = glm::vec4(0, d.r, 0, 1);
    }
    else if(channel == 2)
    {
        d = glm::vec4(0, 0, d.r, 1);
    }
    else if(channel == 3)
    {
        d = glm::vec4(0, 0, 0, d.r);
    }
    else 
    {
        d = vec4(0, 0, 0, 1); 
    }

    imageStore(outputImage, id, d); 
}

)";

bool ChannelMux::Internal::shaderCompiled = false; 

ChannelMux::Internal::Internal()
{
    channel = 0;
}


void ChannelMux::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    
    if(output->GetType() == ImageType::RGB8 || output->GetType() == ImageType::RGBA8)
    {
        if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || input->GetType() != ImageType::GRAYSCALE8) 
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE8); 
        }
    }
    else if(output->GetType() == ImageType::RGB32F || output->GetType() == ImageType::RGBA32F)
    {
        if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight()) 
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE32F); 
        }
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.SetInt("channel", channel); 

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void ChannelMux::Internal::Run(Image* input, Image* output)
{

   
}

void ChannelMux::Internal::SetChannel(int ch)
{
    if(ch < 0)
        ch = 0;
    if(ch > 3)
        ch = 3;

    channel = ch; 
}




ChannelMux::ChannelMux()
{
    internal = new Internal(); 
}

ChannelMux::~ChannelMux()
{ 
    delete internal; 
}

void ChannelMux::SetChannel(int ch)
{
    internal->SetChannel(ch); 
}

void ChannelMux::Run(ImageGPU* input,ImageGPU* output)
{
    internal->Run(input, output); 
}

void ChannelMux::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}