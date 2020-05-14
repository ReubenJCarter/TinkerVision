#include "Mipmaps.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>
#include <math.h>

namespace TnkrVis
{
namespace Process
{

class Mipmaps::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static std::map<ImageType, ComputeShader> copycomputeShaders; 
        static std::string copyshaderSrc; 
        static bool shaderCompiled; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, std::vector<ImageGPU>* output);
        void Run(Image* input, std::vector<Image>* output);
};

std::map<ImageType, ComputeShader> Mipmaps::Internal::computeShaders;

std::string Mipmaps::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

vec4 image2DBilinear(in layout(FORMAT_QUALIFIER) image2D t, in vec2 pos)
{
    vec2 posFloor = floor(pos);
    vec2 posFract = pos - posFloor; 
    ivec2 pfI = ivec2(posFloor); 
    vec4 A = mix( imageLoad(t, pfI),               imageLoad(t, pfI + ivec2(1, 0)), posFract.x );
    vec4 B = mix( imageLoad(t, pfI + ivec2(0, 1)), imageLoad(t, pfI + ivec2(1, 1)), posFract.x );
    return mix(A, B, posFract.y); 
}


layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 outPix = vec4(0, 0, 0, 0);
    
    ivec2 coord = ivec2(id.x * 2, id.y * 2);
    outPix += imageLoad(inputImage, coord + ivec2(0, 0) );  
    outPix += imageLoad(inputImage, coord + ivec2(1, 0) );  
    outPix += imageLoad(inputImage, coord + ivec2(0, 1) );  
    outPix += imageLoad(inputImage, coord + ivec2(1, 1) );  
    
    outPix /= 4; 

    imageStore(outputImage, id, outPix); 
}

)";

std::map<ImageType, ComputeShader> Mipmaps::Internal::copycomputeShaders;

std::string Mipmaps::Internal::copyshaderSrc = R"(
layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    imageStore(outputImage, id, imageLoad(inputImage, id )); 
}

)";

bool Mipmaps::Internal::shaderCompiled = false; 

Mipmaps::Internal::Internal()
{
}


void Mipmaps::Internal::Run(ImageGPU* input, std::vector<ImageGPU>* output) //outputs and image stack!
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        CompileImageComputeShaders(copycomputeShaders, copyshaderSrc);
        shaderCompiled = true; 
    }

    ImageType inputType = input->GetType();
    ComputeShader& computeShader = computeShaders[inputType];
    ComputeShader& copycomputeShader = copycomputeShaders[inputType];

    if(input->GetWidth() == 0 || input->GetHeight() == 0)
    {
        return; 
    }

    //How to calculate mm https://computergraphics.stackexchange.com/questions/1441/how-does-mip-mapping-work-with-non-power-of-2-textures
    int smallestDim = (std::min)(input->GetHeight(), input->GetWidth());
    int maxLevelsBelow = floor(log2((double)smallestDim));

    output->resize(maxLevelsBelow + 1); 
    
    
    //copy first image straight to stack
    output->at(0).Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    copycomputeShader.SetImage("inputImage", input);
    copycomputeShader.SetImage("outputImage", &(output->at(0)), ComputeShader::WRITE_ONLY);
    glm::ivec2 copyGroupCount = ComputeWorkGroupCount(glm::ivec2(output->at(0).GetWidth(), output->at(0).GetHeight()), glm::i32vec2(16, 16)); 
    copycomputeShader.Dispatch(copyGroupCount.x, copyGroupCount.y, 1); 
    copycomputeShader.Block();

    double runningW = output->at(0).GetWidth();
    double runningH = output->at(0).GetHeight();
    for(int i = 1; i <= maxLevelsBelow; i++)
    {
        runningW /= 2;
        runningH /= 2;

        output->at(i).Allocate(runningW, runningH, input->GetType()); 

        computeShader.SetImage("inputImage", &(output->at(i-1)) );
        computeShader.SetImage("outputImage", &(output->at(i)), ComputeShader::WRITE_ONLY);

        glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->at(i).GetWidth(), output->at(i).GetHeight()), glm::i32vec2(16, 16)); 
        computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
        computeShader.Block();
    }
}

void Mipmaps::Internal::Run(Image* input, std::vector<Image>* output)
{
    if(input->GetWidth() == 0 || input->GetHeight() == 0)
    {
        return; 
    }
    int smallestDim = (std::min)(input->GetHeight(), input->GetWidth());
    int maxLevelsBelow = floor(log2((double)smallestDim));
    output->resize(maxLevelsBelow + 1); 

    Image* firstimage = &output->at(0);
    firstimage->Copy(input); 

    ParallelFor& pf = ParallelFor::GetInstance(); 
    
    double runningW = output->at(0).GetWidth();
    double runningH = output->at(0).GetHeight();
    for(int i = 1; i <= maxLevelsBelow; i++)
    {
        runningW /= 2;
        runningH /= 2;
        output->at(i).Allocate(runningW, runningH, input->GetType()); 
        Image* localInput = &(output->at(i-1));
        Image* localOutput = &(output->at(i)); 
        auto kernel = [this, localInput, localOutput](int x, int y)
        {
            glm::vec4 outpix = glm::vec4(0, 0, 0, 0); 
            outpix += GetPixel(localInput, x*2,     y*2); 
            outpix += GetPixel(localInput, x*2 + 1, y*2); 
            outpix += GetPixel(localInput, x*2,     y*2 + 1); 
            outpix += GetPixel(localInput, x*2 + 1, y*2 + 1); 
            outpix /= 4.0f; 
            SetPixel(localOutput, x, y, outpix); 
        };
        pf.Run(localOutput->GetWidth(), localOutput->GetHeight(), kernel);
    }
}



Mipmaps::Mipmaps()
{
    internal = new Internal(); 
}

Mipmaps::~Mipmaps()
{
    delete internal; 
}

void Mipmaps::Run(ImageGPU* input, std::vector<ImageGPU>* output)
{
    internal->Run(input, output); 
}

void Mipmaps::Run(Image* input, std::vector<Image>* output)
{
    internal->Run(input, output); 
}

}
}