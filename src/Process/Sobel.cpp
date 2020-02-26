#include "Sobel.h"

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

class Sobel::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        Mode mode; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetMode(Mode m); 
};

std::map<ImageType, ComputeShader> Sobel::Internal::computeShaders;

std::string Sobel::Internal::shaderSrc = R"(

layout(rgba32f, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

const int MODE_FULL = 0; 
const int MODE_GRAD_ONLY = 1;
const int MODE_MAG_ONLY = 2;

uniform int mode; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    //GRAD X
    float dx = 0;

    dx += imageLoad(inputImage, id + ivec2(-1, -1)).r * 1.0f;
    dx += imageLoad(inputImage, id + ivec2(-1,  0)).r * 2.0f;
    dx += imageLoad(inputImage, id + ivec2(-1,  1)).r * 1.0f;

    dx += imageLoad(inputImage, id + ivec2(1, -1)).r * -1.0f;
    dx += imageLoad(inputImage, id + ivec2(1,  0)).r * -2.0f;
    dx += imageLoad(inputImage, id + ivec2(1,  1)).r * -1.0f;

    //GRAD Y
    float dy = 0;

    dy += imageLoad(inputImage, id + ivec2(-1, -1)).r * 1.0f;
    dy += imageLoad(inputImage, id + ivec2( 0, -1)).r * 2.0f;
    dy += imageLoad(inputImage, id + ivec2( 1, -1)).r * 1.0f;

    dy += imageLoad(inputImage, id + ivec2(-1, 1)).r * -1.0f;
    dy += imageLoad(inputImage, id + ivec2(0,  1)).r * -2.0f;
    dy += imageLoad(inputImage, id + ivec2(1,  1)).r * -1.0f;

    //Normalization Neeeded?? what value ? https://stackoverflow.com/questions/15892116/is-the-sobel-filter-meant-to-be-normalized
    dx /= 8.0f;
    dy /= 8.0f;

    if(mode == MODE_FULL)
    {
        //mag ori calc
        float mag = length(vec2(dx, dy)); 
        float ori = atan(dy, dx); //equivelent of atan2 in glsl ...hmmm

        vec4 outVec = vec4(dx + 0.5f, dy + 0.5f, mag, ori); 
        imageStore(outputImage, id, outVec); 
    }
    else if(mode == MODE_GRAD_ONLY)
    {
        vec4 outVec = vec4(dx + 0.5f, dy + 0.5f, 0, 1); 
        imageStore(outputImage, id, outVec); 
    }
    else if(mode == MODE_MAG_ONLY)
    {
        float mag = length(vec2(dx, dy)); 
        vec4 outVec = vec4(mag, 0, 0, 1); 
        imageStore(outputImage, id, outVec); 
    }
}

)";

bool Sobel::Internal::shaderCompiled = false; 

Sobel::Internal::Internal()
{
    mode = Mode::FULL; 
}

void Sobel::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(mode == Mode::FULL)
    {
        if(output->GetWidth() != input->GetWidth() || 
        output->GetHeight() != input->GetHeight() ||
          !(output->GetType() == ImageType::RGBA32F || 
            output->GetType() == ImageType::RGBA8)) //not either of these two
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); //default to 32bit float 
        }
    }
    else if(mode == Mode::GRAD_ONLY)
    {
        if(output->GetWidth() != input->GetWidth() || 
        output->GetHeight() != input->GetHeight() ||
          !(output->GetType() == ImageType::RGBA32F || 
            output->GetType() == ImageType::RGBA8 || 
            output->GetType() == ImageType::RGB8 ||
            output->GetType() == ImageType::RGB32F)) //not currently one of these these
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGB32F); //default to 32bit float 
        }
    }
    else if(mode == Mode::MAG_ONLY)
    {
        if(output->GetWidth() != input->GetWidth() || 
        output->GetHeight() != input->GetHeight() )      
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE32F); //default to 32bit float 
        }
    }
    
    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Sobel::Internal::Run(Image* input, Image* output)
{
    if(mode == Mode::FULL)
    {
        if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() ||
          !(output->GetType() == ImageType::RGBA32F || 
            output->GetType() == ImageType::RGBA8)) //not either of these two
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); //default to 32bit float 
        }
    }
    else if(mode == Mode::GRAD_ONLY)
    {
        if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() ||
          !(output->GetType() == ImageType::RGBA32F || 
            output->GetType() == ImageType::RGBA8 || 
            output->GetType() == ImageType::RGB8 ||
            output->GetType() == ImageType::RGB32F)) //not currently one of these these
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGB32F); //default to 32bit float 
        }
    }
    else if(mode == Mode::MAG_ONLY)
    {
        if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() )      
        {
            output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE32F); //default to 32bit float 
        }
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        //GRAD X
        float dx = 0;

        dx += GetPixel(input, x-1, y-1).r * 1.0f;
        dx += GetPixel(input, x-1, y+0).r * 2.0f;
        dx += GetPixel(input, x-1, y+1).r * 1.0f;

        dx += GetPixel(input, x+1, y-1).r * -1.0f;
        dx += GetPixel(input, x+1, y+0).r * -2.0f;
        dx += GetPixel(input, x+1, y+1).r * -1.0f;

        //GRAD Y
        float dy = 0;

        dy += GetPixel(input, x-1, y-1).r * 1.0f;
        dy += GetPixel(input, x+0, y-1).r * 2.0f;
        dy += GetPixel(input, x+1, y-1).r * 1.0f;

        dy += GetPixel(input, x-1, y+1).r * -1.0f;
        dy += GetPixel(input, x+0, y+1).r * -2.0f;
        dy += GetPixel(input, x+1, y+1).r * -1.0f;

        //Normalization Neeeded?? https://stackoverflow.com/questions/15892116/is-the-sobel-filter-meant-to-be-normalized
        dx /= 8.0f;
        dy /= 8.0f;

        if(mode == Mode::FULL)
        {
            //mag ori calc
            float mag = glm::length(glm::vec2(dx, dy)); 
            float ori = atan2(dy, dx); 
            glm::vec4 outVec = glm::vec4(dx + 0.5f, dy + 0.5f, mag, ori); 
            SetPixel(output, x, y, outVec); 
        }
        else if(mode == Mode::GRAD_ONLY)
        {
            glm::vec4 outVec = glm::vec4(dx + 0.5f, dy + 0.5f, 0, 1); 
            SetPixel(output, x, y, outVec);  
        }
        else if(mode == Mode::MAG_ONLY)
        {
            float mag = glm::length(glm::vec2(dx, dy)); 
            glm::vec4 outVec = glm::vec4(mag, 0, 0, 1); 
            SetPixel(output, x, y, outVec); 
        }
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void Sobel::Internal::SetMode(Mode m)
{
    mode = m;
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

void Sobel::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Sobel::SetMode(Mode m)
{
    internal->SetMode(m); 
}

}