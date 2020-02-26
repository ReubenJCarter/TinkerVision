#include "AverageFilter.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class AverageFilter::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        int size;
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetSize(int s);
};

std::map<ImageType, ComputeShader> AverageFilter::Internal::computeShaders;

std::string AverageFilter::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform int size; 
uniform int size2; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = vec4(0, 0, 0, 0); 
    for(int x = -size/2; x <= size/2; x++)
    {
        for(int y = -size/2; y <= size/2; y++)
        {
            d += imageLoad(inputImage, id + ivec2(x, y) ); 
        }
    }
    d /= float(size2);
    imageStore(outputImage, id, d); 
}

)";

bool AverageFilter::Internal::shaderCompiled = false; 

AverageFilter::Internal::Internal()
{
    size = 3;
}


void AverageFilter::Internal::Run(ImageGPU* input, ImageGPU* output)
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
    computeShader.SetInt("size2", size * size); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void AverageFilter::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 
    int size2 = size*size;
    auto kernel = [this, input, output, size2](int x, int y)
    {
        glm::vec4 d = glm::vec4(0, 0, 0, 0); 
        for(int dx = -size/2; dx <= size/2; dx++)
        {
            for(int dy = -size/2; dy <= size/2; dy++)
            {
                d += GetPixel(input, x + dx, y + dy);
            }
        }
        d /= size2;
        SetPixel(output, x, y, d); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void AverageFilter::Internal::SetSize(int s)
{
    size = s;
}






AverageFilter::AverageFilter()
{
    internal = new Internal(); 
}

AverageFilter::~AverageFilter()
{
    delete internal; 
}

void AverageFilter::SetSize(int s)
{
    internal->SetSize(s);
}

void AverageFilter::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void AverageFilter::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}