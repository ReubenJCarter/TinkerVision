#include "ClearColor.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

#include <glm/gtc/type_ptr.hpp>

namespace Visi
{

class ClearColor::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        glm::vec4 color;
       
    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* output);
        void Run(Image* output);
        void SetColor(glm::vec4 c);
};

std::map<ImageType, ComputeShader> ClearColor::Internal::computeShaders;

std::string ClearColor::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

vec4 color; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id) * contrast + vec4(brightness, brightness, brightness, 0.0f); 
    imageStore(outputImage, id, d); 
}

)";

bool ClearColor::Internal::shaderCompiled = false; 

ClearColor::Internal::Internal()
{
    color = glm::vec4(0, 0, 0, 0);
}


void ClearColor::Internal::Run(ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }
    if(output->GetWidth() == 0 || output->GetHeight() == 0)
    {

        return; 
    }
    
    ImageType outputType = output->GetType();

    ComputeShader& computeShader = computeShaders[outputType];

    computeShader.SetFloat4("color", glm::value_ptr(color)); 

    computeShader.SetImage("outputImage", output);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void ClearColor::Internal::Run(Image* output)
{
    unsigned char* outputData = output->GetData(); 
    
}

void ClearColor::Internal::SetColor(glm::vec4 c)
{
    color = c;
}




ClearColor::ClearColor()
{
    internal = new Internal(); 
}

ClearColor::~ClearColor()
{
    delete internal; 
}

void ClearColor::SetColor(glm::vec4 c)
{
    internal->SetColor(c);
}

void ClearColor::SetColor(glm::vec3 c)
{
    internal->SetColor(glm::vec4(c.r, c.g, c.b, 1.0));
}

void ClearColor::Run(ImageGPU* output)
{
    internal->Run(output); 
}

void ClearColor::Run(Image* output)
{
    internal->Run(output); 
}

}