#include "MorphRect.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Viso
{
namespace Process
{

class MorphRect::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 
        
        int width;
        int height;  
        Mode mode; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetMode(Mode m);
        void SetKernel(int w, int h);  
};

std::map<ImageType, ComputeShader> MorphRect::Internal::computeShaders;

std::string MorphRect::Internal::shaderSrc = R"(

#define ERODE 0
#define DILATE 1

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform int mode;
uniform int size; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    int halfSize = size/2; 
    vec4 outVec = vec4(0, 0, 0, 1); 

    if(mode == ERODE)
    {
        ivec4 darkDetect = ivec4(0, 0, 0, 0); 

        for(int i = 0; i < size; i++)
        {
            int Y = i - halfSize; 
            for(int j = 0; j < size; j++)
            {
                int X = j - halfSize; 

                ivec2 imageId = id + ivec2(X, Y); 
                vec4 imageIn = imageLoad(inputImage, imageId);
                if(imageIn.r < 0.5)
                    darkDetect.r = 1;
                if(imageIn.g < 0.5)
                    darkDetect.g = 1;
                if(imageIn.b < 0.5)
                    darkDetect.b = 1;
            }
        }

        outVec.r = darkDetect.r > 0.5 ? 0:1; 
        outVec.g = darkDetect.g > 0.5 ? 0:1;
        outVec.b = darkDetect.b > 0.5 ? 0:1;
    }
    else if(mode == DILATE)
    {
        ivec4 lightDetect = ivec4(0, 0, 0, 0); 

        for(int i = 0; i < size; i++)
        {
            int Y = i - halfSize; 
            for(int j = 0; j < size; j++)
            {
                int X = j - halfSize; 

                ivec2 imageId = id + ivec2(X, Y); 
                vec4 imageIn = imageLoad(inputImage, imageId);
                if(imageIn.r >= 0.5)
                    lightDetect.r = 1;
                if(imageIn.g >= 0.5)
                    lightDetect.g = 1;
                if(imageIn.b >= 0.5)
                    lightDetect.b = 1;
            }
        }
        
        outVec.r = lightDetect.r > 0.5 ? 1:0; 
        outVec.g = lightDetect.g > 0.5 ? 1:0;
        outVec.b = lightDetect.b > 0.5 ? 1:0;
    }
    
    
    imageStore(outputImage, id, outVec); 
}

)";

bool MorphRect::Internal::shaderCompiled = false; 

MorphRect::Internal::Internal()
{
    width =3;
    height = 3; 
    mode = Mode::DILATE;
}


void MorphRect::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.SetInt("width", width);
    computeShader.SetInt("height", height);
    computeShader.SetInt("mode", (int)mode);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void MorphRect::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void MorphRect::Internal::SetMode(Mode m)
{
    mode = m;
}

void MorphRect::Internal::SetKernel(int w, int h)
{
    width = w;
    height = h; 
}




MorphRect::MorphRect()
{
    internal = new Internal(); 
}

MorphRect::~MorphRect()
{
    delete internal; 
}

void MorphRect::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void MorphRect::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void MorphRect::SetMode(Mode mode)
{
    internal->SetMode(mode); 
}

void MorphRect::SetKernel(int w, int h)
{
    internal->SetKernel(w, h); 
}

}
}