#include "Crop.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>


namespace Visi
{

class Crop::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled;   

        int offset[2]; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetOffset(int x, int y);
};

std::map<ImageType, ComputeShader> Crop::Internal::computeShaders;

std::string Crop::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform ivec2 offset;

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
    
    ivec2 coord = id + offset;
    outPix = imageLoad(inputImage, coord );  
    
    imageStore(outputImage, id, outPix); 
}

)";

bool Crop::Internal::shaderCompiled = false; 

Crop::Internal::Internal()
{
}


void Crop::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetInt2("offset", offset); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Crop::Internal::Run(Image* input, Image* output)
{

    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        glm::vec4 pix = GetPixel(input, x + offset[0], y + offset[1]); 
        SetPixel(output, x, y, pix); 
    };

    pf.Run(output->GetWidth(), output->GetHeight(), kernel);
}

void Crop::Internal::SetOffset(int x, int y)
{
    offset[0] = x; 
    offset[1] = y; 
}


Crop::Crop()
{
    internal = new Internal(); 
}

Crop::~Crop()
{
    delete internal; 
}

void Crop::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output);
}

void Crop::Run(Image* input, Image* output)
{   
    internal->Run(input, output);
}

void Crop::SetOffset(int x, int y)
{
    internal->SetOffset(x, y); 
}
	
}