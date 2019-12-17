#include "NonMaximumSuppression.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class NonMaximumSuppression::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled;        
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
};

std::map<ImageType, ComputeShader> NonMaximumSuppression::Internal::computeShaders;

std::string NonMaximumSuppression::Internal::shaderSrc = R"(

layout(FORMAT_QUALIFIER, binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

const float sin45 = 0.70710678118;
const float cos45 = 0.70710678118;

vec4 BilinearPixelQuad(ivec2 id, vec4 centerPixelValue, vec2 gradNorm)
{
    //grab the quad of pixel value we will do bilinear interpolation over
    int cornerX = gradNorm.x >= 0.0f ? 1 : -1; 
    int cornerY = gradNorm.y >= 0.0f ? 1 : -1; 
    ivec2 corner = ivec2(cornerX, cornerY);

    //grab all the quad values
    vec4 cornerVal = imageLoad(inputImage, id + corner);
    vec4 topVal = imageLoad(inputImage, id + ivec2(0, corner.y));
    vec4 sideVal = imageLoad(inputImage, id + ivec2(corner.x, 0));

    //abs the grad norm vector to use as bilinear interp factors
    vec2 gradNormAbs = abs(gradNorm); 
    vec4 bilinX0 = mix(centerPixelValue, sideVal, gradNormAbs.x); 
    vec4 bilinX1 = mix(topVal, cornerVal, gradNormAbs.x); 
    vec4 bilin = mix(bilinX0, bilinX1, gradNormAbs.y); 

    return bilin;
}

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 d = imageLoad(inputImage, id);

    vec2 gradNorm = vec2(d.x, d.y) / d.z; 

    vec4 v0 = BilinearPixelQuad(id, d, gradNorm); 
    vec4 v1 = BilinearPixelQuad(id, d, -gradNorm); 

    if(d.z <= v0.z || d.z < v1.z)
        d = vec4(0, 0, 0, 0);

    imageStore(outputImage, id, d); 
}

)";

bool NonMaximumSuppression::Internal::shaderCompiled = false; 

NonMaximumSuppression::Internal::Internal()
{
}


void NonMaximumSuppression::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(!(input->GetType() == ImageType::RGBA32F || input->GetType() == ImageType::RGBA8))
    {
        return; 
    }

    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void NonMaximumSuppression::Internal::Run(Image* input, Image* output)
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



NonMaximumSuppression::NonMaximumSuppression()
{
    internal = new Internal(); 
}

NonMaximumSuppression::~NonMaximumSuppression()
{
    delete internal; 
}

void NonMaximumSuppression::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void NonMaximumSuppression::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}