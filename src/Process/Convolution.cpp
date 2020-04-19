#include "Convolution.h"

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

class Convolution::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* conv, ImageGPU* output);
        void Run(Image* input, Image* conv, Image* output);
};

std::map<ImageType, ComputeShader> Convolution::Internal::computeShaders;

std::string Convolution::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D convImage;

uniform int convW;
uniform int convH;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id) * contrast + vec4(brightness, brightness, brightness, 0.0f); 
    imageStore(outputImage, id, d); 
}

)";

bool Convolution::Internal::shaderCompiled = false; 

Convolution::Internal::Internal()
{
}


void Convolution::Internal::Run(ImageGPU* input, ImageGPU* conv, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetInt("convW", conv->GetWidth()); 
    computeShader.SetInt("convH", conv->GetHeight()); 
    computeShader.SetImage("convImage", conv);
    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Convolution::Internal::Run(Image* input, Image* conv, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, conv, output](int x, int y)
    {
        glm::vec4 pix = GetPixel(input, x, y); 
        glm::vec4 d;
        SetPixel(output, x, y, d); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}





Convolution::Convolution()
{
    internal = new Internal(); 
}

Convolution::~Convolution()
{
    delete internal; 
}


void Convolution::Run(ImageGPU* input, ImageGPU* conv, ImageGPU* output)
{
    internal->Run(input, conv, output); 
} 

void Convolution::Run(Image* input, Image* conv, Image* output)
{
    internal->Run(input, conv, output); 
}

}
}