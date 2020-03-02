#include "HSVToRGB.h"

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

class HSVToRGB::Internal
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

std::map<ImageType, ComputeShader> HSVToRGB::Internal::computeShaders;

//http://www.chilliant.com/rgb2hsv.html
std::string HSVToRGB::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;


vec3 HUEtoRGB(in float H)
{
    float R = abs(H * 6 - 3) - 1;
    float G = 2 - abs(H * 6 - 2);
    float B = 2 - abs(H * 6 - 4);
    return clamp(vec3(R,G,B), 0.0f, 1.0f);
}

vec3 HSVToRGB(vec3 HSV)
{
    vec3 RGB = HUEtoRGB(HSV.x);
    return ((RGB - 1) * HSV.y + 1) * HSV.z;
}

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    vec3 rgb = HSVToRGB(d.xyz); 
    imageStore(outputImage, id, vec4(rgb, d.a)); 
}

)";

bool HSVToRGB::Internal::shaderCompiled = false; 

HSVToRGB::Internal::Internal()
{
}


void HSVToRGB::Internal::Run(ImageGPU* input, ImageGPU* output)
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

void HSVToRGB::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 d = GetPixel(input, x, y); 
        
        glm::vec3 HSV = d;
        float H = d.x; 

        float R = abs(H * 6 - 3) - 1;
        float G = 2 - abs(H * 6 - 2);
        float B = 2 - abs(H * 6 - 4);

        glm::vec3 RGB = glm::clamp(glm::vec3(R,G,B), 0.0f, 1.0f);

        glm::vec3 rgb = ((RGB - glm::vec3(1, 1, 1) ) * HSV.y + glm::vec3(1, 1, 1)) * HSV.z;

        SetPixel(output, x, y, glm::vec4(rgb, d.a)); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel); 
}




HSVToRGB::HSVToRGB()
{
    internal = new Internal(); 
}

HSVToRGB::~HSVToRGB()
{ 
    delete internal; 
}

void HSVToRGB::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void HSVToRGB::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}