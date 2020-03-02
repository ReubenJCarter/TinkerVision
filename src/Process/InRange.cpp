#include "InRange.h"

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

class InRange::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        glm::vec3 highThreshold; 
        glm::vec3 lowThreshold; 
        int invertRange[3]; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetHighThreshold(float tH);
        void SetLowThreshold(float tL); 
        void SetHighThreshold(glm::vec3 tH);
        void SetLowThreshold(glm::vec3 tL);
        void SetInvertRange(bool invCh0, bool invCh1, bool invCh2); 
};

std::map<ImageType, ComputeShader> InRange::Internal::computeShaders;

std::string InRange::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform vec3 lowThreshold; 
uniform vec3 highThreshold;
uniform ivec3 invertRange; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    
    vec4 outVec ; 
    bvec3 inRange; 
    inRange.r = invertRange.r != 0 ? !(d.r < highThreshold.r && d.r >= lowThreshold.r) : (d.r < highThreshold.r && d.r >= lowThreshold.r);
    inRange.g = invertRange.g != 0 ? !(d.g < highThreshold.g && d.g >= lowThreshold.g) : (d.g < highThreshold.g && d.g >= lowThreshold.g);
    inRange.b = invertRange.b != 0 ? !(d.b < highThreshold.b && d.b >= lowThreshold.b) : (d.b < highThreshold.b && d.b >= lowThreshold.b);

    outVec.r = (inRange.r && inRange.g && inRange.b) ? 1.0f : 0.0f;
    outVec.a = 1; 
    imageStore(outputImage, id, outVec);
}

)";

bool InRange::Internal::shaderCompiled = false; 

InRange::Internal::Internal()
{
    lowThreshold = glm::vec3(0.5, 0.5, 0.5);
    highThreshold = glm::vec3(0.5, 0.5, 0.5); 
    invertRange[0] = 0;
    invertRange[1] = 0;
    invertRange[2] = 0; 
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

    computeShader.SetInt3("invertRange", invertRange); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void InRange::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::GRAYSCALE8)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE8); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 d = GetPixel(input, x, y); 
        
        glm::vec4 outVec ; 
        bool inRange[3]; 
        inRange[0] = invertRange[0] != 0 ? !(d.r < highThreshold.r && d.r >= lowThreshold.r) : (d.r < highThreshold.r && d.r >= lowThreshold.r);
        inRange[1] = invertRange[0] != 0 ? !(d.g < highThreshold.g && d.g >= lowThreshold.g) : (d.g < highThreshold.g && d.g >= lowThreshold.g);
        inRange[2] = invertRange[0] != 0 ? !(d.b < highThreshold.b && d.b >= lowThreshold.b) : (d.b < highThreshold.b && d.b >= lowThreshold.b);

        outVec.r = (inRange[0] && inRange[1] && inRange[2]) ? 1.0f : 0.0f;
        outVec.a = 1; 
        
        SetPixel(output, x, y, d); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
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

void InRange::Internal::SetInvertRange(bool invCh0, bool invCh1, bool invCh2)
{
    invertRange[0] = invCh0 ? 1 : 0; 
    invertRange[1] = invCh1 ? 1 : 0; 
    invertRange[2] = invCh2 ? 1 : 0; 
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

void InRange::SetLowThreshold(Vec3 t)
{
	internal->SetLowThreshold(glm::vec3(t.x, t.y, t.z));
}

void InRange::SetHighThreshold(float t)
{
    internal->SetHighThreshold(t);
}

void InRange::SetHighThreshold(Vec3 t)
{
	internal->SetHighThreshold(glm::vec3(t.x, t.y, t.z));
}

void InRange::SetInvertRange(bool invCh0, bool invCh1, bool invCh2)
{
    internal->SetInvertRange(invCh0, invCh1, invCh2);
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
}