#include "Blend.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class Blend::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        BlendMode blendMode;
       
    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output);
        void Run(Image* inputSrc, Image* inputDst, Image* output);

        void SetMode(BlendMode bm); 
};

std::map<ImageType, ComputeShader> Blend::Internal::computeShaders;

std::string Blend::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputSrcImage;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D inputDstImage;

const uint ADDITION = 0; 
const uint SUBTRACT = 1;
const uint DIFFERENCE = 2;
const uint DIVIDE = 3;
const uint MULTIPLY = 4;
const uint ALPHABLEND = 5;

uniform int blendMode;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 dSrc = imageLoad(inputSrcImage, id);
    vec4 dDst = imageLoad(inputDstImage, id);
    vec4 d;

    if(blendMode == int(ADDITION))
    {   
        d = dDst + dSrc; 
    }
    else if(blendMode == int(SUBTRACT))
    {
        d = dDst - dSrc; 
    }
    else if(blendMode == int(DIFFERENCE))
    {
        d.r = length(dDst.r - dSrc.r);
        d.g = length(dDst.g - dSrc.g);
        d.b = length(dDst.b - dSrc.b);
    }
    else if(blendMode == int(DIVIDE))
    {
        d.r = dDst.r / dSrc.r;
        d.g = dDst.g / dSrc.g;
        d.b = dDst.b / dSrc.b;
    }
    else if(blendMode == int(MULTIPLY))
    {
        d.r = dDst.r * dSrc.r;
        d.g = dDst.g * dSrc.g;
        d.b = dDst.b * dSrc.b;
    }
    else if(blendMode == int(ALPHABLEND))
    {
        d.r = dDst.r * (1.0f-dSrc.a) + dSrc.r * dSrc.a;
        d.g = dDst.g * (1.0f-dSrc.a) + dSrc.g * dSrc.a;
        d.b = dDst.b * (1.0f-dSrc.a) + dSrc.b * dSrc.a;
        d.a = dDst.a * (1.0f-dSrc.a) + dSrc.a * dSrc.a;
    }
    else 
    {
        d = vec4(1, 0, 0, 1); 
    }

    imageStore(outputImage, id, d); 
}

)";

bool Blend::Internal::shaderCompiled = false; 

Blend::Internal::Internal()
{
    blendMode = BlendMode::ALPHABLEND;
}


void Blend::Internal::Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!inputSrc->IsSameDimensions(inputDst))
    {
        std::cout << "inputSrc is not the same dimentions as inputDst\n";
        return; 
    }

    if(!output->IsSameDimensions(inputSrc)) 
    {
        output->Allocate(inputSrc->GetWidth(), inputSrc->GetHeight(), inputSrc->GetType()); 
    }

    ImageType inputType = inputSrc->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputSrcImage", inputSrc);
    computeShader.SetImage("inputDstImage", inputDst);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.SetInt("blendMode", blendMode); 

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(inputSrc->GetWidth(), inputSrc->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Blend::Internal::Run(Image* inputSrc, Image* inputDst, Image* output)
{

    if(!inputSrc->IsSameDimensions(inputDst))
    {
        std::cout << "inputSrc is not the same dimentions as inputDst\n";
        return; 
    }

    if(!output->IsSameDimensions(inputSrc)) 
    {
        output->Allocate(inputSrc->GetWidth(), inputSrc->GetHeight(), inputSrc->GetType()); 
    }
    
    unsigned char* inputSrcData = inputSrc->GetData(); 
    unsigned char* inputDstData = inputDst->GetData(); 
    unsigned char* outputData = output->GetData(); 
    for(int i = 0; i < inputSrc->GetHeight(); i++)
    {
        for(int j = 0; j < inputSrc->GetWidth(); j++)
        {
            int inx = (i * inputSrc->GetWidth() + j);
        } 
    } 
}

void Blend::Internal::SetMode(BlendMode bm)
{
    blendMode = bm; 
}




Blend::Blend()
{
    internal = new Internal(); 
}

Blend::~Blend()
{ 
    delete internal; 
}

void Blend::SetMode(BlendMode bm)
{
    internal->SetMode(bm); 
}

void Blend::Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output)
{
    internal->Run(inputSrc, inputDst, output); 
}

void Blend::Run(Image* inputSrc, Image* inputDst, Image* output)
{
    internal->Run(inputSrc, inputDst, output); 
}

}