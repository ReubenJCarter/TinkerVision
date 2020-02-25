//Thanks to http://mccormickml.com/2014/01/10/stereo-vision-tutorial-part-i/

#include "StereoPairDepth.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

class StereoPairDepth::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

    public:
        Internal(); 
        void Run(ImageGPU* inputL, ImageGPU* inputR, ImageGPU* output); 
        void Run(Image* inputL, Image* inputR, Image* output); 
};


std::map<ImageType, ComputeShader> StereoPairDepth::Internal::computeShaders;

std::string StereoPairDepth::Internal::shaderSrc = R"(

layout(rgba32f, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImageL;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D inputImageR;

uniform int blockW;
uniform int blockH;
uniform int blockA; 
uniform int halfBlockW;
uniform int halfBlockH;
uniform float maxK; 
uniform float camSeparation;
uniform float camFocal;
uniform float pixelScale;

float DepthFromCorrespondence(float cor)
{
    return (camSeparation * camFocal) / (cor * pixelScale);
}

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    float cachel[blockA];
	float lowestsad = 1000.0f;
	int lowestsadK = 0;
	float sad = 0;

    vec4 pixL = imageLoad(inputImageL, id );
    vec4 pixR = imageLoad(inputImageL, id );
    
    imageStore(outputImage, id, outVec); 
}

)";

bool StereoPairDepth::Internal::shaderCompiled = false; 

void StereoPairDepth::Internal::Run(ImageGPU* inputL, ImageGPU* inputR, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!inputL->IsSameDimensions(inputR))
    {
        std::cout << "Visi:StereoPairDepth:inputL is not the same dimentions as inputR\n";
        return;
    }

    if(output->GetWidth() != inputL->GetWidth() || 
       output->GetHeight() != inputL->GetHeight() || 
       output->GetType() != ImageType::GRAYSCALE32F)
    {
        output->Allocate(inputL->GetWidth(), inputL->GetHeight(), ImageType::GRAYSCALE16); 
    }
    
    ImageType inputType = inputL->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(inputL->GetWidth(), inputL->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImageL", inputL);
    computeShader.SetImage("inputImageR", inputR);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void StereoPairDepth::Internal::Run(Image* inputL, Image* inputR, Image* output)
{
    if(!inputL->IsSameDimensions(inputR))
    {
        std::cout << "Visi:StereoPairDepth:inputL is not the same dimentions as inputR\n";
        return;
    }

    if(output->GetWidth() != inputL->GetWidth() || 
       output->GetHeight() != inputL->GetHeight() || 
       output->GetType() != ImageType::GRAYSCALE32F)
    {
        output->Allocate(inputL->GetWidth(), inputL->GetHeight(), ImageType::GRAYSCALE16); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, inputL, inputR, output](int x, int y)
    {
        glm::vec4 outVec = GetPixel(inputL, x, y);
       
        SetPixel(output, x, y, outVec); 
    };

    pf.Run(inputL->GetWidth(), inputL->GetHeight(), kernel);
}






StereoPairDepth::StereoPairDepth()
{
    internal = new Internal(); 
}

StereoPairDepth::~StereoPairDepth()
{
    delete internal; 
}

void StereoPairDepth::Run(ImageGPU* inputL, ImageGPU* inputR, ImageGPU* output)
{
    internal->Run(inputL, inputR, output); 
}

void StereoPairDepth::Run(Image* inputL, Image* inputR, Image* output)
{
    internal->Run(inputL, inputR, output); 
}

}