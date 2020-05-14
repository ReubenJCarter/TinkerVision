#include "Rotate.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace TnkrVis
{
namespace Process
{


class Rotate::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        double rotationDeg; 
        bool resizeToFit;

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetRotation(double r){rotationDeg=r;}
        void SetResizeToFit(bool r){resizeToFit = r;}
};

std::map<ImageType, ComputeShader> Rotate::Internal::computeShaders;

std::string Rotate::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform mat2 rotMat; 
uniform vec2 offset; 

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
    vec2 idf = vec2(float(id.x), float(id.y)) - offset; 
    idf = (rotMat * idf) ; 
    vec4 d = image2DBilinear(inputImage, idf); 
    imageStore(outputImage, id, d); 
}

)";

bool Rotate::Internal::shaderCompiled = false; 

Rotate::Internal::Internal()
{
    rotationDeg = 0; 
    resizeToFit = true; 
}

void Rotate::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    
    
    double DEGTORAD = 0.01745329251; 
    double ang = -DEGTORAD * rotationDeg; 
    glm::mat2 rotMat = glm::mat2(glm::vec2(cos(ang),  sin(ang)), 
                                 glm::vec2(-sin(ang), cos(ang))   );

    glm::mat2 rotMatInv = glm::mat2(glm::vec2(cos(-ang),  sin(-ang)), 
                                 glm::vec2(-sin(-ang), cos(-ang))   );

    glm::vec2 a = glm::vec2(0, 0); 
    glm::vec2 b = rotMatInv * glm::vec2(input->GetWidth(), 0); 
    glm::vec2 c = rotMatInv * glm::vec2(input->GetWidth(), input->GetHeight()); 
    glm::vec2 d = rotMatInv * glm::vec2(0, input->GetHeight()); 

    glm::vec2 aabbMin;
    glm::vec2 aabbMax;

    aabbMin.x = (std::min)( (std::min)(a.x, b.x), (std::min)(c.x, d.x) );
    aabbMin.y = (std::min)( (std::min)(a.y, b.y), (std::min)(c.y, d.y) );

    aabbMax.x = (std::max)( (std::max)(a.x, b.x), (std::max)(c.x, d.x) );
    aabbMax.y = (std::max)( (std::max)(a.y, b.y), (std::max)(c.y, d.y) );

    int newW = aabbMax.x - aabbMin.x;
    int newH = aabbMax.y - aabbMin.y;

    if(resizeToFit)
    {
        ReallocateIfNotSameSize(output, newW, newH, input->GetType() ); 
    }
    else
    {
        ReallocateIfNotSameSize(output, input); 
    }


    glm::vec2 offset = -aabbMin;  

    ImageType inputType = input->GetType();

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetMat2("rotMat", glm::value_ptr( rotMat ) ); 
    computeShader.SetFloat2("offset", glm::value_ptr( offset ) ); 
    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void Rotate::Internal::Run(Image* input, Image* output)
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




Rotate::Rotate()
{
    internal = new Internal; 

}

Rotate::~Rotate()
{
    delete internal; 
}

void Rotate::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Rotate::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Rotate::SetRotation(double degs)
{
    internal->SetRotation(degs); 
}

void Rotate::SetResizeToFit(bool r)
{
    internal->SetResizeToFit(r); 
}
	
}
}