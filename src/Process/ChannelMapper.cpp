#include "ChannelMapper.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>


namespace Visi
{

class ChannelMapper::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        glm::ivec4 channelMap;
        glm::vec4 defaultChannelValues;
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetChannelMap(int r, int g, int b, int a); 
};

std::map<ImageType, ComputeShader> ChannelMapper::Internal::computeShaders;

std::string ChannelMapper::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage0;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D inputImage1;
layout(FORMAT_QUALIFIER, binding=3) uniform image2D inputImage2;
layout(FORMAT_QUALIFIER, binding=4) uniform image2D inputImage3;

uniform ivec4 channelMap; 
uniform vec4 defaultChannelValues; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);    
    vec4 outPix;


    vec4 d = imageLoad(inputImage0, id); 
    outPix.r = channelMap.r >= 0 ? d[channelMap.r] : defaultChannelValues.r; 
    outPix.g = channelMap.g >= 0 ? d[channelMap.g] : defaultChannelValues.g; 
    outPix.b = channelMap.b >= 0 ? d[channelMap.b] : defaultChannelValues.b; 
    outPix.a = channelMap.a >= 0 ? d[channelMap.a] : defaultChannelValues.a; 


    imageStore(outputImage, id, outPix); 
}

)";

bool ChannelMapper::Internal::shaderCompiled = false; 

ChannelMapper::Internal::Internal()
{
    channelMap = glm::ivec4(0, 1, 2, 3);
    defaultChannelValues = glm::vec4(0, 0, 0, 1);
}


void ChannelMapper::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight()) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), output->GetType()); 
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetInt4("channelMap", glm::value_ptr(channelMap)); 
    computeShader.SetFloat4("defaultChannelValues", glm::value_ptr(defaultChannelValues));

    computeShader.SetImage("inputImage0", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void ChannelMapper::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight()) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), output->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 
    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 outPix;

        glm::vec4 d = glm::vec4(0, 0, 0, 0); 
        outPix.r = channelMap.r >= 0 ? d[channelMap.r] : defaultChannelValues.r; 
        outPix.g = channelMap.g >= 0 ? d[channelMap.g] : defaultChannelValues.g; 
        outPix.b = channelMap.b >= 0 ? d[channelMap.b] : defaultChannelValues.b; 
        outPix.a = channelMap.a >= 0 ? d[channelMap.a] : defaultChannelValues.a; 

        SetPixel(output, x, y, outPix); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void ChannelMapper::Internal::SetChannelMap(int r, int g, int b, int a)
{
    channelMap = glm::ivec4(r, g, b, a);
}






ChannelMapper::ChannelMapper()
{
    internal = new Internal(); 
}

ChannelMapper::~ChannelMapper()
{
    delete internal; 
}

void ChannelMapper::SetChannelMap(int r, int g, int b, int a)
{
    internal->SetChannelMap(r, g, b, a);
}

void ChannelMapper::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void ChannelMapper::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}