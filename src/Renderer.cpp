#include "Renderer.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>

namespace Visi
{

class Renderer::Internal
{
    private:
        struct Circle
        {
            glm::vec4 color;
            glm::vec2 centre;
            float radius;
            float borderWidth; 
            int filled; 
        }; 

        struct Line
        {
            glm::vec4 color; 
            glm::vec2 start;
            glm::vec2 end; 
            float borderWidth; 
            int filled; 
        }; 

        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        bool circlesDirty; 
        std::vector<Circle> circles; 
        ShaderStorageBuffer circlesShaderBuf; 

    public:
        Internal(); 
        void CompileComputeShaders(std::string sSrc); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void Clear(); 
        void AddCircle(glm::vec2 centre, float radius, glm::vec4 color=glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), bool filled=false, float borderWidth=1); 

};

std::map<ImageType, ComputeShader> Renderer::Internal::computeShaders;

std::string Renderer::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

struct Circle
{
    vec4 color; 
    vec2 centre;
    float radius;
    float borderWidth;
    int filled;
};

layout (std430) buffer circlesBlock
{
	Member circles[];
};
uniform int circlesCount;

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id); 
    
    for(int i = 0; i < circlesCount; i++)
    {
        Circle c = circles[i];
        float dist = 
    }
    
    imageStore(outputImage, id, d); 
}

)";

bool Renderer::Internal::shaderCompiled = false; 

Renderer::Internal::Internal()
{
    circlesDirty = true; 
}


void Renderer::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    if(circlesDirty)
    {
        circlesShaderBuf.Allocate(sizeof(Circle) * circles.size(), 0); 
        circlesShaderBuf.Copy((unsigned char*)&circles[0]); 
        circlesDirty = false; 
    }
    computeShader.SetInt("circlesCount", circles.size() );
    computeShader.SetShaderStorageBuffer("circlesBlock", &circlesShaderBuf); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Renderer::Internal::Run(Image* input, Image* output)
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

void Renderer::Internal::Clear()
{
    circlesDirty = true; 
}

void Renderer::Internal::AddCircle(glm::vec2 centre, float radius, glm::vec4 color, bool filled, float borderWidth)
{
    Circle c;
    c.borderWidth = borderWidth;
    c.centre = centre;
    c.color = color; 
    c.filled = filled ? 1:0;
    c.radius = radius; 
    circles.push_back(c); 
    circlesDirty = true;
}




Renderer::Renderer()
{
    internal = new Internal(); 
}

Renderer::~Renderer()
{
    delete internal; 
}

void Renderer::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Renderer::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Renderer::AddCircle(glm::vec2 centre, float radius, glm::vec4 color, bool filled, float borderWidth)
{
    internal->AddCircle(centre, radius, color, filled, borderWidth); 
}

}