#include "Flip.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace TnkrVis
{
namespace Process
{


class Flip::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        bool flipHorizontal;
        bool flipVertical;

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetDirection(bool flipHor, bool flipVer){flipHorizontal=flipHor; flipVertical=flipVer;}
};

std::map<ImageType, ComputeShader> Flip::Internal::computeShaders;

std::string Flip::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform int flipHor; 
uniform int flipVer; 

uniform int texWidth;
uniform int texHeight; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    ivec2 id2;
    id2.x = flipHor == 1 ? (texWidth - 1) - id.x : id.x;
    id2.y = flipVer == 1 ? (texHeight - 1) - id.y : id.y;
    vec4 d = imageLoad(inputImage, id2 );
    imageStore(outputImage, id, d); 
}

)";

bool Flip::Internal::shaderCompiled = false; 

Flip::Internal::Internal()
{
    flipHorizontal = true; 
    flipVertical = false;
}

void Flip::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }
    
    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetInt("flipHor", flipHorizontal ? 1:0 ); 
    computeShader.SetInt("flipVer", flipVertical ? 1:0 ); 
    computeShader.SetInt("texWidth", input->GetWidth() ); 
    computeShader.SetInt("texHeight", input->GetHeight() ); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Flip::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {


    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}




Flip::Flip()
{
    internal = new Internal; 
}

Flip::~Flip()
{
    delete internal; 
}

void Flip::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Flip::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Flip::SetDirection(bool flipHor, bool flipVer)
{
    internal->SetDirection(flipHor, flipVer);  
}

	
}
}