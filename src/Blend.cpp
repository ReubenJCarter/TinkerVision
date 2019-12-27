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
        void Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output, ImageGPU* blendMask=NULL);
        void Run(Image* inputSrc, Image* inputDst, Image* output, Image* blendMask=NULL);

        void SetMode(BlendMode bm); 
};

std::map<ImageType, ComputeShader> Blend::Internal::computeShaders;

std::string Blend::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputSrcImage;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D inputDstImage;
layout(FORMAT_QUALIFIER, binding=3) uniform image2D inputMaskImage;

const uint ADDITION = 0; 
const uint SUBTRACT = 1;
const uint DIFFERENCE = 2;
const uint DIVIDE = 3;
const uint MULTIPLY = 4;

uniform int blendMode;

uniform int useMask; 

vec3 ApplyMask(vec3 src, vec3 dst, vec3 a)
{
    vec3 pixout; 
    pixout.r = a.r * src.r + (1.0f - a.r) * dst.r;
    pixout.g = a.r * src.g + (1.0f - a.r) * dst.g;
    pixout.b = a.r * src.b + (1.0f - a.r) * dst.b;
    return pixout;
}

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    vec4 blendMask = vec4(1, 1, 1, 1); 
    if(useMask != 0)
    {
        blendMask = imageLoad(inputMaskImage, id);
    }

    vec4 dSrc = imageLoad(inputSrcImage, id);
    vec4 dDst = imageLoad(inputDstImage, id);
    vec4 d;
    d.a = 1.0f; 

    if(blendMode == int(BLEND_ADD))
    {   
        d = dDst + dSrc; 

        d.rgb = ApplyMask(d.rgb, dDst.rgb, blendMask.rgb); 
    }
    else if(blendMode == int(BLEND_SUBTRACT))
    {
        d = dDst - dSrc; 

        d.rgb = ApplyMask(d.rgb, dDst.rgb, blendMask.rgb); 
    }
    else if(blendMode == int(BLEND_DIFFERENCE))
    {
        d.r = length(dDst.r - dSrc.r);
        d.g = length(dDst.g - dSrc.g);
        d.b = length(dDst.b - dSrc.b);

        d.rgb = ApplyMask(d.rgb, dDst.rgb, blendMask.rgb); 
    }
    else if(blendMode == int(BLEND_DIVIDE))
    {
        d.r = dDst.r / dSrc.r;
        d.g = dDst.g / dSrc.g;
        d.b = dDst.b / dSrc.b;

        d.rgb = ApplyMask(d.rgb, dDst.rgb, blendMask.rgb); 
    }
    else if(blendMode == int(BLEND_MULTIPLY))
    {
        d.r = dDst.r * dSrc.r;
        d.g = dDst.g * dSrc.g;
        d.b = dDst.b * dSrc.b;

        d.rgb = ApplyMask(d.rgb, dDst.rgb, blendMask.rgb); 
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
    blendMode = BlendMode::BLEND_ADD;
}


void Blend::Internal::Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output, ImageGPU* blendMask)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!inputSrc->IsSameDimensions(inputDst))
    {
        std::cout << "Visi:Blend:Run:inputSrc is not the same dimentions as inputDst\n";
        return; 
    }

    if(!output->IsSameDimensions(inputSrc)) 
    {
        output->Allocate(inputSrc->GetWidth(), inputSrc->GetHeight(), inputSrc->GetType()); 
    }

    ImageType inputType = inputSrc->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    if(blendMask != NULL)
    {
        if(blendMask->GetWidth() == inputSrc->GetWidth() && blendMask->GetHeight() == inputSrc->GetHeight())
        {
            computeShader.SetInt("useMask", 1); 
            computeShader.SetImage("inputMaskImage", blendMask);
        }
        else
        {
            std::cout << "Visi:Blend:Run:blendMask is not the same dimentions as input\n";
            return; 
        }
    }
    else
    {
        computeShader.SetInt("useMask", 0); 
    }
    computeShader.SetImage("inputSrcImage", inputSrc);
    computeShader.SetImage("inputDstImage", inputDst);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.SetInt("blendMode", blendMode); 

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(inputSrc->GetWidth(), inputSrc->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Blend::Internal::Run(Image* inputSrc, Image* inputDst, Image* output, Image* blendMask)
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

void Blend::Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output, ImageGPU* blendMask)
{
    internal->Run(inputSrc, inputDst, blendMask, output); 
}

void Blend::Run(Image* inputSrc, Image* inputDst, Image* output, Image* blendMask)
{
    internal->Run(inputSrc, inputDst, blendMask, output); 
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