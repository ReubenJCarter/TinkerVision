#include "RGBToHSV.h"

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

class RGBToHSV::Internal
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

std::map<ImageType, ComputeShader> RGBToHSV::Internal::computeShaders;

//http://www.chilliant.com/rgb2hsv.html
std::string RGBToHSV::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

float Epsilon = 1e-10;

vec3 RGBtoHCV(vec3 RGB)
{
    // Based on work by Sam Hocevar and Emil Persson
    vec4 P = (RGB.g < RGB.b) ? vec4(RGB.bg, -1.0f, 2.0f/3.0f) : vec4(RGB.gb, 0.0f, -1.0f/3.0f);
    vec4 Q = (RGB.r < P.x) ? vec4(P.xyw, RGB.r) : vec4(RGB.r, P.yzx);
    float C = Q.x - min(Q.w, Q.y);
    float H = abs((Q.w - Q.y) / (6 * C + Epsilon) + Q.z);
    return vec3(H, C, Q.x);
}

vec3 RGBtoHSV(vec3 RGB)
{
    vec3 HCV = RGBtoHCV(RGB);
    float S = HCV.y / (HCV.z + Epsilon);
    return vec3(HCV.x, S, HCV.z);
}

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    vec3 hsv = RGBtoHSV(d.rgb); 
    imageStore(outputImage, id, vec4(hsv, d.a)); 
}

)";

bool RGBToHSV::Internal::shaderCompiled = false; 

RGBToHSV::Internal::Internal()
{
}


void RGBToHSV::Internal::Run(ImageGPU* input, ImageGPU* output)
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

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void RGBToHSV::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 
    float Epsilon = 1e-10;

    auto kernel = [this, input, output, Epsilon](int x, int y)
    {
        glm::vec4 RGBA = GetPixel(input, x, y);
        glm::vec3 RGB = RGBA;

        glm::vec4 P = (RGB.g < RGB.b) ? glm::vec4(RGB.b, RGB.g, -1.0f, 2.0f/3.0f) : glm::vec4(RGB.g, RGB.b, 0.0f, -1.0f/3.0f);
        glm::vec4 Q = (RGB.r < P.x) ? glm::vec4(P.x, P.y, P.w, RGB.r) : glm::vec4(RGB.r, P.y, P.z, P.x);
        float C = Q.x - (std::min)(Q.w, Q.y);
        float H = abs((Q.w - Q.y) / (6 * C + Epsilon) + Q.z);
        glm::vec3 HCV =  glm::vec3(H, C, Q.x);

        float S = HCV.y / (HCV.z + Epsilon);
        glm::vec4 outVec = glm::vec4(HCV.x, S, HCV.z, RGBA.a);

        SetPixel(output, x, y, outVec); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}




RGBToHSV::RGBToHSV()
{
    internal = new Internal(); 
}

RGBToHSV::~RGBToHSV()
{ 
    delete internal; 
}

void RGBToHSV::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void RGBToHSV::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}