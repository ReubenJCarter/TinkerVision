#include "CircleHoughTransform.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace TnkrVis
{
namespace Process
{

class CircleHoughTransform::Internal
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

std::map<ImageType, ComputeShader> CircleHoughTransform::Internal::computeShaders;

std::string CircleHoughTransform::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage; //Output here does not need a format qualifier because its write only
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    float av = (d.r + d.g + d.b ) / 3.0f; 
    imageStore(outputImage, id, vec4(av, av, av, av)); 
}

)";

bool CircleHoughTransform::Internal::shaderCompiled = false; 

CircleHoughTransform::Internal::Internal()
{
}


void CircleHoughTransform::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }
    ReallocateIfNotSameSize(output, input,  ImageType::GRAYSCALE16); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void CircleHoughTransform::Internal::Run(Image* input, Image* output)
{ 
    
    ReallocateIfNotSameSize(output, input,  ImageType::GRAYSCALE16); 
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 d = GetPixel(input, x, y); 
        
        glm::vec4 outVec ; 

        float av = (d.r + d.g + d.b ) / 3.0f; 
        outVec.r = av;
        outVec.g = av;
        outVec.b = av;
        outVec.a = d.a;

        SetPixel(output, x, y, outVec); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel); 
}




CircleHoughTransform::CircleHoughTransform()
{
    internal = new Internal(); 
}

CircleHoughTransform::~CircleHoughTransform()
{ 
    delete internal; 
}

void CircleHoughTransform::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void CircleHoughTransform::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}