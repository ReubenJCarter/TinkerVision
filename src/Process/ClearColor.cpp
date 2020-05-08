#include "ClearColor.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

#include <glm/gtc/type_ptr.hpp>

namespace Viso
{
namespace Process
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
        void Run(ImageGPU* output);
        void Run(Image* output);
        void SetColor(glm::vec4 c);
};

std::map<ImageType, ComputeShader> ClearColor::Internal::computeShaders;

std::string ClearColor::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;

uniform vec4 color; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    imageStore(outputImage, id, color); 
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
    if(output->GetWidth() == 0 || output->GetHeight() == 0)
    {

        return; 
    }

    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, output](int x, int y)
    {
        SetPixel(output, x, y, color); 
    };

    pf.Run(output->GetWidth(), output->GetHeight(), kernel);
    
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

void ClearColor::SetColor(float r, float g, float b, float a)
{
    internal->SetColor(glm::vec4(r, g, b, a));
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
}