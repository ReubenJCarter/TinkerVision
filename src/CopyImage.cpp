#include "CopyImage.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class CopyImage::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled;    
        
        bool formatTranslate; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetFormatTranslate(bool ftr);
};

std::map<ImageType, ComputeShader> CopyImage::Internal::computeShaders;

std::string CopyImage::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);
    imageStore(outputImage, id, d); 
}

)";

bool CopyImage::Internal::shaderCompiled = false; 

CopyImage::Internal::Internal()
{
    formatTranslate = true; 
}


void CopyImage::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }


    if(formatTranslate)
    {
        if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() )
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), output->GetType()); 
        }
    }
    else
    {
        if(!output->IsSameDimensions(input)) 
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
        }
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void CopyImage::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    for(int i = 0; i < input->GetHeight(); i++)
    {
        for(int j = 0; j < input->GetWidth(); j++)
        {
            int inx = (i * input->GetWidth() + j);


        } 
    } 
}

void CopyImage::Internal::SetFormatTranslate(bool ftr)
{
    formatTranslate = ftr;
}



CopyImage::CopyImage()
{
    internal = new Internal(); 
}

CopyImage::~CopyImage()
{
    delete internal; 
}

void CopyImage::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void CopyImage::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void CopyImage::SetFormatTranslate(bool ftr)
{
    internal->SetFormatTranslate(ftr);
}

}