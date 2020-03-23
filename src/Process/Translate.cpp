#include "Translate.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{
namespace Process
{


class Translate::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        glm::vec2 translation; 
        bool resizeToFit;

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetTranslation(glm::vec2 t){translation=t;}
        void SetResizeToFit(bool r){resizeToFit = r;}
};

std::map<ImageType, ComputeShader> Translate::Internal::computeShaders;

std::string Translate::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform vec2 translation; 

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
    vec2 idf = vec2(float(id.x), float(id.y)) - translation; 
    vec4 d = image2DBilinear(inputImage, idf); 
    imageStore(outputImage, id, d); 
}

)";

bool Translate::Internal::shaderCompiled = false; 

Translate::Internal::Internal()
{
    translation = glm::vec2(0, 0); 
    resizeToFit = true;
}

void Translate::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

   
    int newW = input->GetWidth() + abs(translation.x);
    int newH = input->GetHeight() + abs(translation.y);

    if(resizeToFit)
    {
        ReallocateIfNotSameSize(output, newW, newH, input->GetType() ); 
    }
    else
    {
        ReallocateIfNotSameSize(output, input); 
    }

    if(translation.x < 0)translation.x = 0;
    if(translation.y < 0)translation.y = 0;

    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetFloat2("translation", glm::value_ptr( translation ) ); 
    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Translate::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    auto kernel = [this, input, output](int x, int y)
    {
        //GRAD X
        float dx = 0;


    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}




Translate::Translate()
{
    internal = new Internal; 

}

Translate::~Translate()
{
    delete internal; 
}

void Translate::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Translate::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Translate::SetTranslation(float x, float y)
{
    internal->SetTranslation(glm::vec2(x, y) ); 
}

void Translate::SetResizeToFit(bool r)
{
    internal->SetResizeToFit(r); 
}
	
}
}