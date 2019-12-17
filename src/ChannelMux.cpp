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
	vec4 dout = imageLoad(outImage, id);

    if(channel == 0)
    {
        dout.r = d.r;
    }
    else if(channel == 1)
    {
		dout.g = d.r;
    }
    else if(channel == 2)
    {
		dout.b = d.r;
    }
    else if(channel == 3)
    {
		dout.a = d.r;
    }

    imageStore(outputImage, id, dout); 
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
	
	if(input->GetWidth() != output->GetWidth() || input->GetHeight() != output->GetHeight() || output->GetWidth() == 0 || output->GetHeight() == 0)
	{
		
		return; 
	}

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output);

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