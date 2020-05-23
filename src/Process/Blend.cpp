#include "Blend.h"

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

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputSrcImage;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D inputDstImage;
layout(FORMAT_QUALIFIER, binding=3) uniform image2D inputMaskImage;

const uint BLEND_ADD = 0; 
const uint BLEND_SUBTRACT = 1;
const uint BLEND_DIFFERENCE = 2;
const uint BLEND_DIVIDE = 3;
const uint BLEND_MULTIPLY = 4;
const uint BLEND_OVERLAY = 5;

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
        d.rgb = dDst.rgb + dSrc.rgb; 

        d.rgb = ApplyMask(d.rgb, dDst.rgb, blendMask.rgb); 
    }
    else if(blendMode == int(BLEND_SUBTRACT))
    {
        d.rgb = dDst.rgb - dSrc.rgb; 

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
    else if(blendMode == int(BLEND_OVERLAY))
    {
        d.rgb = ApplyMask(dSrc.rgb, dDst.rgb, blendMask.rgb); 
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
        std::cout << "TnkrVis:Blend:Run:inputSrc is not the same dimentions as inputDst\n";
        return; 
    }

    ReallocateIfNotSameSize(output, inputSrc); 

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
            std::cout << "TnkrVis:Blend:Run:blendMask is not the same dimentions as input\n";
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
        std::cout << "TnkrVis:Blend:inputSrc is not the same dimentions as inputDst\n";
        return; 
    }

    ReallocateIfNotSameSize(output, inputDst); 

    bool useMask = false; 
    if(blendMask != NULL)
    {
        if(blendMask->GetWidth() == inputSrc->GetWidth() && blendMask->GetHeight() == inputSrc->GetHeight())
        {
            useMask = true; 
        }
        else
        {
            std::cout << "TnkrVis:Blend:Run:blendMask is not the same dimentions as input\n";
            return; 
        }
    }
    
   
        ParallelFor& pf = ParallelFor::GetInstance(); 

        auto kernel = [this, inputSrc, inputDst, blendMask, useMask, output](int x, int y)
        {
            glm::vec3 pixSrc = GetPixel(inputSrc, x, y); 
            glm::vec3 pixDst = GetPixel(inputDst, x, y); 
            glm::vec3 d ; 

            if(blendMode == BlendMode::BLEND_ADD)
            {
                d = pixDst + pixSrc;    
            }
            else if(blendMode == BlendMode::BLEND_SUBTRACT)
            {
                d =  pixDst - pixSrc;  
            }
            else if(blendMode == BlendMode::BLEND_DIFFERENCE)
            {
                d =  glm::abs(pixDst - pixSrc);  
            }
            else if(blendMode == BlendMode::BLEND_DIVIDE)
            {
                d = pixDst / pixSrc;
            }
            else if(blendMode == BlendMode::BLEND_MULTIPLY)
            {
                d = pixDst * pixSrc;
            }
            else if(blendMode == BlendMode::BLEND_OVERLAY)
            {
                d = pixSrc; 
            }

            if(useMask)
            {
                glm::vec3 pixMask = GetPixel(blendMask, x, y); 
                d = glm::mix(pixDst, d, pixMask.r); 
            }
            d = glm::clamp(d, glm::vec3( 0, 0, 0), glm::vec3( 1, 1, 1));
            SetPixel(output, x, y, glm::vec4(d, 1)); 
        };

        pf.Run(inputSrc->GetWidth(), inputSrc->GetHeight(), kernel);
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

void Blend::Run(ImageGPU* inputTop, ImageGPU* inputBottom, ImageGPU* output, ImageGPU* blendMask)
{
    internal->Run(inputTop, inputBottom, output, blendMask); 
}

void Blend::Run(Image* inputTop, Image* inputBottom, Image* output, Image* blendMask)
{
    internal->Run(inputTop, inputBottom, output, blendMask); 
}

void Blend::Run(ImageGPU* inputTop, ImageGPU* inputBottom, ImageGPU* output)
{
    internal->Run(inputTop, inputBottom, output); 
}

void Blend::Run(Image* inputTop, Image* inputBottom, Image* output)
{
    internal->Run(inputTop, inputBottom, output); 
}

}
}