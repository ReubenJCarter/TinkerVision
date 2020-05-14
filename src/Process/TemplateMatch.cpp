#include "TemplateMatch.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"
#include "../Process/OtsuThreshold.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace TnkrVis
{
namespace Process
{

class TemplateMatch::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders; 
        static std::string shaderSrc; 
        static bool shaderCompiled; 
        MatchMode matchMode; 
        bool normalized; 
        glm::vec4 kernelSum; 
        ImageGPU* matchImageGPURef; 
        Image* matchImageRef; 

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* match, ImageGPU* output);
        void Run(Image* input, Image* match, Image* output);
        void SetMatchMode(MatchMode mm); 
        void SetNormalized(bool n); 
};

std::map<ImageType, ComputeShader> TemplateMatch::Internal::computeShaders;

std::string TemplateMatch::Internal::shaderSrc = R"(

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;
layout(FORMAT_QUALIFIER, binding=2) uniform image2D matchImage;

const uint MATCH_SAD = 0; 
const uint MATCH_CORR = 1;

uniform int matchImageWidth;
uniform int matchImageHeight;
uniform int mode; 
uniform vec4 kernelSum; 

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    ivec2 mS = ivec2(matchImageWidth/2, matchImageHeight/2); 
    if( mode == int(MATCH_SAD) )
    {
        ivec2 id = ivec2(gl_GlobalInvocationID.xy);
        vec4 sum = vec4(0, 0, 0, 0); 
        for(int j = -mS.y; j <= mS.y; j++)
        {
            for(int i = -mS.x; i <= mS.x; i++)
            {
                ivec2 offset = ivec2(i, j); 
                sum += abs( imageLoad(matchImage, offset + mS) - imageLoad(inputImage, id + offset) );
            }
        }
        sum /= kernelSum;
        imageStore(outputImage, id, sum); 
    }
    else if( mode == int(MATCH_CORR) )
    {
        ivec2 id = ivec2(gl_GlobalInvocationID.xy);
        vec4 sum = vec4(0, 0, 0, 0); 
        for(int j = -mS.y; j <= mS.y; j++)
        {
            for(int i = -mS.x; i <= mS.x; i++)
            {
                ivec2 offset = ivec2(i, j); 
                sum += abs( imageLoad(matchImage, offset + mS) * imageLoad(inputImage, id + offset) );
            }
        }
        sum /= kernelSum;
        imageStore(outputImage, id, sum); 
    }
}

)";

bool TemplateMatch::Internal::shaderCompiled = false; 

TemplateMatch::Internal::Internal()
{
    matchMode = MatchMode::MATCH_SAD; 
    normalized = true; 
    matchImageGPURef = NULL; 
}

void TemplateMatch::Internal::Run(ImageGPU* input, ImageGPU* match, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders, shaderSrc); 
        shaderCompiled = true; 
    }

    if(match->GetType() != input->GetType())
    {
        std::cerr << "TnkrVis:Process:TemplateMatch:input and match image type do not match FAIL\n"; 
        return;
    }
    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();

    ComputeShader& computeShader = computeShaders[inputType];

    //recompute kkernel sum if match image changed and normalize is on
    if(matchImageGPURef != match && normalized)
    {
        Image temp; 
        temp.Copy(match);  
        glm::vec4 sum(0, 0, 0, 0);
        for(int j = 0; j < temp.GetHeight(); j++)
        {
            for(int i = 0; i < temp.GetWidth(); i++)
            {
                sum += GetPixel(&temp, i, j);
            }
        }
        kernelSum = sum;
        matchImageGPURef = match; 
    }
    else if(!normalized)
    {
        kernelSum = glm::vec4(1, 1, 1, 1);
    }

    computeShader.SetInt("mode", matchMode); 
    computeShader.SetInt("matchImageWidth", match->GetWidth() ); 
    computeShader.SetInt("matchImageHeight", match->GetHeight() ); 
    computeShader.SetFloat4("kernelSum", glm::value_ptr(kernelSum)); 
    computeShader.SetImage("inputImage", input, ComputeShader::READ_ONLY);
    computeShader.SetImage("matchImage", match, ComputeShader::READ_ONLY); 
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(output->GetWidth(), output->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void TemplateMatch::Internal::Run(Image* input, Image* match, Image* output)
{
    ReallocateIfNotSameSize(output, input); 

    //recompute kkernel sum if match image changed and normalize is on
    if(matchImageRef != match && normalized)
    {
        glm::vec4 sum(0, 0, 0, 0);
        for(int j = 0; j < match->GetHeight(); j++)
        {
            for(int i = 0; i < match->GetWidth(); i++)
            {
                sum += GetPixel(match, i, j);
            }
        }
        kernelSum = sum;
        matchImageRef = match; 
    }
    else if(!normalized)
    {
        kernelSum = glm::vec4(1, 1, 1, 1);
    }

    ParallelFor& pf = ParallelFor::GetInstance(); 

    int matchImageWidth = match->GetWidth();
    int matchImageHeight = match->GetHeight();
    glm::ivec2 mS = glm::ivec2(matchImageWidth/2, matchImageHeight/2); 

    auto kernel = [this, input, output, match, matchImageWidth, matchImageHeight, mS](int x, int y)
    {
        if( matchMode == MATCH_SAD )
        {
            glm::vec4 sum = glm::vec4(0, 0, 0, 0); 
            for(int j = -mS.y; j <= mS.y; j++)
            {
                for(int i = -mS.x; i <= mS.x; i++)
                {
                    sum += abs( GetPixel(match, i + mS.x, j + mS.y) - GetPixel(input, x + i, y + j) );
                }
            }
            SetPixel(output, x, y, sum); 
        }
        else if( matchMode == MATCH_CORR )
        {
            glm::vec4 sum = glm::vec4(0, 0, 0, 0); 
            for(int j = -mS.y; j <= mS.y; j++)
            {
                for(int i = -mS.x; i <= mS.x; i++)
                {
                    sum += abs( GetPixel(match, i + mS.x, j + mS.y) * GetPixel(input, x + i, y + j) );
                }
            }
            SetPixel(output, x, y, sum);  
        }

        
    };

    pf.Run(input->GetWidth(), input->GetHeight(), kernel);
}

void TemplateMatch::Internal::SetMatchMode(MatchMode mm)
{
    matchMode = mm; 
}

void TemplateMatch::Internal::SetNormalized(bool n)
{
    normalized = n; 
}


TemplateMatch::TemplateMatch()
{
    internal = new Internal(); 
}

TemplateMatch::~TemplateMatch()
{
    delete internal; 
}

void TemplateMatch::SetMatchMode(MatchMode mm)
{
    internal->SetMatchMode(mm); 
}

void TemplateMatch::SetNormalized(bool n)
{
    internal->SetNormalized(n); 
}

void TemplateMatch::Run(ImageGPU* input, ImageGPU* match, ImageGPU* output)
{
    internal->Run(input, match, output); 
}

void TemplateMatch::Run(Image* input, Image* match, Image* output)
{
    internal->Run(input, match, output); 
}

}
}