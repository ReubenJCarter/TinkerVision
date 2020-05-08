#include "HighLowThreshold.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Viso
{
namespace Process
{

class HighLowThreshold::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        glm::vec3 highThreshold; 
        glm::vec3 lowThreshold; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetHighThreshold(float tH);
        void SetLowThreshold(float tL); 
        void SetHighThreshold(glm::vec3 tH);
        void SetLowThreshold(glm::vec3 tL);
};

std::map<ImageType, ComputeShader> HighLowThreshold::Internal::computeShaders;

std::string HighLowThreshold::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform vec3 lowThreshold; 
uniform vec3 highThreshold;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    
    vec4 outVec ; 

    outVec.r = d.r < highThreshold.r && d.r >= lowThreshold.r ? 1.0f : 0.0f;
    outVec.g = d.g < highThreshold.g && d.g >= lowThreshold.g ? 1.0f : 0.0f;
    outVec.b = d.b < highThreshold.b && d.b >= lowThreshold.b ? 1.0f : 0.0f;

    outVec.a = d.a;

    imageStore(outputImage, id, outVec);
}

)";

bool HighLowThreshold::Internal::shaderCompiled = false; 

HighLowThreshold::Internal::Internal()
{
    lowThreshold = glm::vec3(0.5, 0.5, 0.5);
    highThreshold = glm::vec3(0.5, 0.5, 0.5); 
}


void HighLowThreshold::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetFloat3("lowThreshold", glm::value_ptr(lowThreshold)); 
    computeShader.SetFloat3("highThreshold", glm::value_ptr(highThreshold)); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void HighLowThreshold::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 d = GetPixel(input, x, y); 
        
        glm::vec4 outVec ; 

        outVec.r = d.r < highThreshold.r && d.r >= lowThreshold.r ? 1.0f : 0.0f;
        outVec.g = d.g < highThreshold.g && d.g >= lowThreshold.g ? 1.0f : 0.0f;
        outVec.b = d.b < highThreshold.b && d.b >= lowThreshold.b ? 1.0f : 0.0f;

        outVec.a = d.a;

        SetPixel(output, x, y, outVec); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel); 
}

void HighLowThreshold::Internal::SetLowThreshold(float t)
{
    lowThreshold = glm::vec3(t, t, t);
}

void HighLowThreshold::Internal::SetLowThreshold(glm::vec3 t)
{
    lowThreshold = t;
}

void HighLowThreshold::Internal::SetHighThreshold(float t)
{
    highThreshold = glm::vec3(t, t, t);
}

void HighLowThreshold::Internal::SetHighThreshold(glm::vec3 t)
{
    highThreshold = t;
}




HighLowThreshold::HighLowThreshold()
{
    internal = new Internal(); 
}

HighLowThreshold::~HighLowThreshold()
{
    delete internal; 
}

void HighLowThreshold::SetLowThreshold(float t)
{
    internal->SetLowThreshold(t);
}

void HighLowThreshold::SetLowThreshold(Color t)
{
    internal->SetLowThreshold(glm::vec4(t.r, t.g, t.b, t.a));
}

void HighLowThreshold::SetHighThreshold(float t)
{
    internal->SetHighThreshold(t);
}

void HighLowThreshold::SetHighThreshold(Color t)
{
    internal->SetHighThreshold(glm::vec4(t.r, t.g, t.b, t.a));
}

void HighLowThreshold::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void HighLowThreshold::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}