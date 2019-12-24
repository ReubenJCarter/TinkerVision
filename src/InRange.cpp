#include "InRange.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class InRange::Internal
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

std::map<ImageType, ComputeShader> InRange::Internal::computeShaders;

std::string InRange::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform vec3 lowThreshold; 
uniform vec3 highThreshold;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    
    vec4 outVec ; 

    outVec.r = d.r < highThreshold.r && d.r >= lowThreshold.r &&
               d.g < highThreshold.g && d.g >= lowThreshold.g &&
               d.b < highThreshold.b && d.b >= lowThreshold.b 
               ? 1.0f : 0.0f;
    outVec.a = 1; 
    imageStore(outputImage, id, outVec);
}

)";

bool InRange::Internal::shaderCompiled = false; 

InRange::Internal::Internal()
{
    lowThreshold = glm::vec3(0.5, 0.5, 0.5);
    highThreshold = glm::vec3(0.5, 0.5, 0.5); 
}


void InRange::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::GRAYSCALE8)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE8); 
    }

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

void InRange::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    for(int i = 0; i < input->GetHeight(); i++)
    {
        for(int j = 0; j < input->GetWidth(); j++)
        {
            int inx = (i * input->GetWidth() + j);


        } 
    } 
}

void InRange::Internal::SetLowThreshold(float t)
{
    lowThreshold = glm::vec3(t, t, t);
}

void InRange::Internal::SetLowThreshold(glm::vec3 t)
{
    lowThreshold = t;
}

void InRange::Internal::SetHighThreshold(float t)
{
    highThreshold = glm::vec3(t, t, t);
}

void InRange::Internal::SetHighThreshold(glm::vec3 t)
{
    highThreshold = t;
}




InRange::InRange()
{
    internal = new Internal(); 
}

InRange::~InRange()
{
    delete internal; 
}

void InRange::SetLowThreshold(float t)
{
    internal->SetLowThreshold(t);
}

void InRange::SetLowThreshold(glm::vec3 t)
{
    internal->SetLowThreshold(t);
}

void InRange::SetHighThreshold(float t)
{
    internal->SetHighThreshold(t);
}

void InRange::SetHighThreshold(glm::vec3 t)
{
    internal->SetHighThreshold(t);
}

void InRange::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void InRange::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}