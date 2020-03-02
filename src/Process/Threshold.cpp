#include "Threshold.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class Threshold::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        glm::vec3 threshold; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetThreshold(float t);
        void SetThreshold(glm::vec3 t);
};

std::map<ImageType, ComputeShader> Threshold::Internal::computeShaders;

std::string Threshold::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform vec3 threshold; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    d.r = d.r <= threshold.r ? 0.0f : 1.0f; 
    d.g = d.g <= threshold.g ? 0.0f : 1.0f; 
    d.b = d.b <= threshold.b ? 0.0f : 1.0f; 
    imageStore(outputImage, id, d); 
}

)";

bool Threshold::Internal::shaderCompiled = false; 

Threshold::Internal::Internal()
{
    threshold = glm::vec3(0.5, 0.5, 0.5);
}


void Threshold::Internal::Run(ImageGPU* input, ImageGPU* output)
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

    computeShader.SetFloat3("threshold", glm::value_ptr(threshold)); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Threshold::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 pix = GetPixel(input, x, y); 
        pix.r = pix.r <= threshold.r ? 0.0f : 1.0f; 
        pix.g = pix.g <= threshold.g ? 0.0f : 1.0f; 
        pix.b = pix.b <= threshold.b ? 0.0f : 1.0f; 
        SetPixel(output, x, y, pix); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void Threshold::Internal::SetThreshold(float t)
{
    threshold = glm::vec3(t, t, t);
}

void Threshold::Internal::SetThreshold(glm::vec3 t)
{
    threshold = t;
}



Threshold::Threshold()
{
    internal = new Internal(); 
}

Threshold::~Threshold()
{
    delete internal; 
}

void Threshold::SetThreshold(float t)
{
    internal->SetThreshold(t);
}

void Threshold::SetThreshold(Color t)
{
    internal->SetThreshold(glm::vec4(t.r, t.g, t.b, t.a));
}

void Threshold::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Threshold::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}