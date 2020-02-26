#include "UpSample.h"

#include <glm/gtc/type_ptr.hpp>

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class UpSample::Internal
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

std::map<ImageType, ComputeShader> UpSample::Internal::computeShaders;

std::string UpSample::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform vec2 scale; 
uniform vec2 boxSize; //box size in the input image
uniform ivec2 boxSizeI; 
uniform int mode; 

const int MODE_NEAREST = 0; 
const int MODE_BILINEAR = 1;

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
    
    if(mode == MODE_NEAREST)
    {
        ivec2 coord = ivec2(id.x * boxSize.x, id.y * boxSize.y);
        outPix = imageLoad(inputImage, coord );  
    }
    else if(mode == MODE_BILINEAR)
    {     
        vec2 coord = vec2(id.x * boxSize.x, id.y * boxSize.y);
        outPix = image2DBilinear(inputImage, coord );        
    }

    imageStore(outputImage, id, outPix); 
}

)";

bool UpSample::Internal::shaderCompiled = false; 

UpSample::Internal::Internal()
{
    mode = Mode::BILINEAR;
}


void UpSample::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    glm::vec2 scale; 
    scale.x = (float)output->GetWidth() / input->GetWidth(); 
    scale.y = (float)output->GetHeight() / input->GetHeight(); 
    if(scale.x < 1)
    {
        return;
    }
    if(scale.y < 1)
    {
        return;
    }

    if(output->GetType() != input->GetType()) 
    {
        output->Allocate(output->GetWidth(), output->GetHeight(), input->GetType()); 
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    
    computeShader.SetFloat2("scale", glm::value_ptr(scale)); 

    glm::vec2 boxSize = glm::vec2(1.0f / scale.x, 1.0f / scale.y);
    glm::ivec2 boxSizeI = glm::vec2((int)boxSize.x, (int)boxSize.y);
    computeShader.SetFloat2("boxSize", glm::value_ptr(boxSize)); 
    computeShader.SetInt2("boxSizeI", glm::value_ptr(boxSizeI)); 

    computeShader.SetInt("mode", mode); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void UpSample::Internal::Run(Image* input, Image* output)
{

    glm::vec2 scale; 
    scale.x = (float)output->GetWidth() / (float)input->GetWidth(); 
    scale.y = (float)output->GetHeight() / (float)input->GetHeight(); 
    if(scale.x > 1)
    {
        return;
    }
    if(scale.y > 1)
    {
        return;
    }

    if(output->GetType() != input->GetType()) 
    {
        output->Allocate(output->GetWidth(), output->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 pix = GetPixel(input, x, y); 
        SetPixel(output, x, y, pix); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void UpSample::Internal::SetMode(Mode m)
{
    mode = m;
}



UpSample::UpSample()
{
    internal = new Internal(); 
}

UpSample::~UpSample()
{
    delete internal; 
}

void UpSample::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void UpSample::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void UpSample::SetMode(Mode m)
{
    internal->SetMode(m); 
}

}