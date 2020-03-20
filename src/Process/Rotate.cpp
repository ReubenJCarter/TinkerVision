#include "Rotate.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{
namespace Process
{


class Rotate::Internal
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

std::map<ImageType, ComputeShader> Rotate::Internal::computeShaders;

std::string Rotate::Internal::shaderSrc = R"(

layout(rgba32f, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

const int MODE_FULL = 0; 
const int MODE_GRAD_ONLY = 1;
const int MODE_MAG_ONLY = 2;

uniform int mode; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

}

)";

bool Rotate::Internal::shaderCompiled = false; 

Rotate::Internal::Internal()
{
}

void Rotate::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight())
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Rotate::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight())
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        //GRAD X
        float dx = 0;


    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}




Rotate::Rotate()
{
    internal = new Internal; 

}

Rotate::~Rotate()
{
    delete internal; 
}

void Rotate::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Rotate::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}
	
}
}