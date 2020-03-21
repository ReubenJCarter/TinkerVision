#include "CameraDistortion.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class CameraDistortion::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 

        float focalLength; 
        float k[3]; //radial distortion coerf
        float p[2]; //tangential distortion coef
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetRadialCoefs(float k0, float k1, float k2);
        void SetTangentialCoefs(float p0, float p1);
        void SetFocalLength(float fl); 
};

std::map<ImageType, ComputeShader> CameraDistortion::Internal::computeShaders;

//http://homepages.inf.ed.ac.uk/rbf/HIPR2/adpthrsh.htm
std::string CameraDistortion::Internal::shaderSrc = R"(

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

uniform float focalLength; 
uniform vec3 k; 
uniform vec2 p;
uniform vec2 imSize; 
uniform vec2 imCenter; 

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
    vec2 basePos = gl_GlobalInvocationID.xy;

    //convert to normalized image plane coordinates (plane is at z = 1)
    basePos = ((basePos) / (imSize.y * 0.5f)) * focalLength; 
    vec2 centeroffset = basePos - ((imCenter) / (imSize.y * 0.5f)) * focalLength; 

    float r2 = centeroffset.x * centeroffset.x + centeroffset.y * centeroffset.y;  
    float r4 = r2 * r2;
    float r6 = r4 * r2; 
    vec2 radialDistTerm = centeroffset * (k.x * r2 + k.y * r4 + k.z * r6);
    float tangDistTermX = p.x * (r2+2.0f*centeroffset.x*centeroffset.x) + p.y*2.0f*centeroffset.x*centeroffset.y; 
    float tangDistTermY = p.y * (r2+2.0f*centeroffset.y*centeroffset.y) + p.x*2.0f*centeroffset.x*centeroffset.y; 
    vec2 correctedPos = basePos + radialDistTerm + vec2(tangDistTermX, tangDistTermY); 

    vec2 imCoords = (correctedPos / focalLength) * (imSize.y * 0.5f);
    vec4 pix = image2DBilinear(inputImage, imCoords);
    imageStore(outputImage, ivec2(gl_GlobalInvocationID.xy), pix); 
    
}

)";

bool CameraDistortion::Internal::shaderCompiled = false; 

CameraDistortion::Internal::Internal()
{
    focalLength = 1;
    k[0] = 0;
    k[1] = 0;
    k[2] = 0;
    p[0] = 0; 
    p[1] = 0; 
}


void CameraDistortion::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    computeShader.SetFloat("focalLength", focalLength); 
    computeShader.SetFloat3("k", k);
    computeShader.SetFloat2("p", p);
    float sz[] = {input->GetWidth(), input->GetHeight()};  
    computeShader.SetFloat2("imSize", sz); 
    float ctr[] = {input->GetWidth()/2, input->GetHeight()/2};  
    computeShader.SetFloat2("imCenter", ctr); 

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void CameraDistortion::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    ParallelFor& pf = ParallelFor::GetInstance(); 

    glm::vec2 imCenter = glm::vec2(input->GetWidth()/2, input->GetHeight()/2);

    auto kernel = [this, input, output, imCenter](int x, int y)
    {        
        glm::vec2 basePos = glm::vec2(x, y); 

        //convert to normalized image plane coordinates (plane is at z = 1)
        basePos = ((basePos) / (input->GetHeight() * 0.5f)) * focalLength; 
        glm::vec2 centeroffset = basePos - ((imCenter) / (input->GetHeight() * 0.5f)) * focalLength; 

        float r2 = centeroffset.x * centeroffset.x + centeroffset.y * centeroffset.y;  
        float r4 = r2 * r2;
        float r6 = r4 * r2; 
        glm::vec2 radialDistTerm = centeroffset * (k[0] * r2 + k[1] * r4 + k[2] * r6);
        float tangDistTermX = p[0] * (r2+2.0f*centeroffset.x*centeroffset.x) + p[1] * 2.0f * centeroffset.x * centeroffset.y; 
        float tangDistTermY = p[1] * (r2+2.0f*centeroffset.y*centeroffset.y) + p[0] * 2.0f * centeroffset.x * centeroffset.y; 
        glm::vec2 correctedPos = basePos + radialDistTerm + glm::vec2(tangDistTermX, tangDistTermY); 

        glm::vec2 imCoords = (correctedPos / focalLength) * (input->GetHeight() * 0.5f);
        glm::vec4 pix = GetPixelBilinear(input, imCoords.x, imCoords.y);

        SetPixel(output, x, y, pix); 
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void CameraDistortion::Internal::SetRadialCoefs(float k0, float k1, float k2)
{
    k[0] = k0;
    k[1] = k1;
    k[2] = k2; 
}

void CameraDistortion::Internal::SetTangentialCoefs(float p0, float p1)
{
    p[0] = p0;
    p[1] = p1;
}

void CameraDistortion::Internal::SetFocalLength(float fl)
{
    focalLength = fl; 
}



CameraDistortion::CameraDistortion()
{
    internal = new Internal(); 
}

CameraDistortion::~CameraDistortion()
{
    delete internal; 
}

void CameraDistortion::SetRadialCoefs(float k0, float k1, float k2)
{
    internal->SetRadialCoefs(k0, k1, k2);
}

void CameraDistortion::SetTangentialCoefs(float p0, float p1)
{
    internal->SetTangentialCoefs(p0, p1);
}

void CameraDistortion::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void CameraDistortion::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void CameraDistortion::SetFocalLength(float fl)
{
    internal->SetFocalLength(fl); 
}

}
}