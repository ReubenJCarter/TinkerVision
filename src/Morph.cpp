#include "Morph.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

class Morph::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 
        
        Shape shape; 
        int size; 
        Mode mode; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetMode(Mode m);
        void SetKernel(int si, Shape sh);  
};

std::map<ImageType, ComputeShader> Morph::Internal::computeShaders;

std::string Morph::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    
    vec4 outVec = vec4(dx, dy, mag, ori); 
    imageStore(outputImage, id, outVec); 
}

)";

bool Morph::Internal::shaderCompiled = false; 

Morph::Internal::Internal()
{
}


void Morph::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!input->IsSameDimensions(output))
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

void Morph::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void Morph::Internal::SetMode(Mode m)
{
    mode = m;
}

void Morph::Internal::SetKernel(int si, Shape sh)
{
    shape = sh;
    size = si;


}




Morph::Morph()
{
    internal = new Internal(); 
}

Morph::~Morph()
{
    delete internal; 
}

void Morph::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Morph::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Morph::SetMode(Mode mode)
{
    internal->SetMode(mode); 
}

void Morph::SetKernel(int size, Shape shape)
{
    internal->SetKernel(size, shape); 
}

}