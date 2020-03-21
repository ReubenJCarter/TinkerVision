#include "Invert.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class Invert::Internal
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

std::map<ImageType, ComputeShader> Invert::Internal::computeShaders;

std::string Invert::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    d.rgb = vec3(1.0f) - d.rgb; 
    imageStore(outputImage, id, d); 
}

)";

bool Invert::Internal::shaderCompiled = false; 

Invert::Internal::Internal()
{
}


void Invert::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    
    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Invert::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 pix = GetPixel(input, x, y); 
        glm::vec4 d = glm::vec4(1.0f - pix.r, 1.0f - pix.g, 1.0f - pix.b, pix.a);
        SetPixel(output, x, y, d); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}




Invert::Invert()
{
    internal = new Internal(); 
}

Invert::~Invert()
{ 
    delete internal; 
}

void Invert::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Invert::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}