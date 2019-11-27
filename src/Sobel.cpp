#include "Sobel.h"

#include "ComputeShader.h"

#include <string>
#include <iostream>

namespace Visi
{

class Sobel::Internal
{
    private:
        static bool shaderCompiled; 
        static ComputeShader computeShader; 
        static std::string shaderSrc; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
};

std::string Sobel::Internal::shaderSrc = R"(
layout(binding=0) uniform image2D inputImage;
layout(rgba, binding=0) uniform image2D outputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    
}

)";

bool Sobel::Internal::shaderCompiled = false; 

Sobel::Internal::Internal()
{
}

void Sobel::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        computeShader.Compile(shaderSrc);
        shaderCompiled = true; 
    }

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output);

    int targetWidth = input->GetWidth(); 
    int targetHeight = input->GetHeight(); 

    int groupSize = 16;

    int groupW = targetWidth / groupSize; 
    int groupH = targetHeight / groupSize; 

    if(targetWidth % groupSize != 0)
        groupW++;
    if(targetHeight % groupSize != 0)
        groupH++;   

    computeShader.Dispatch(groupW, groupH, 1); 
    computeShader.Block();
}




Sobel::Sobel()
{
    internal = new Internal(); 
}

Sobel::~Sobel()
{
    delete internal; 
}

void Sobel::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

}