#include "MedianFilter.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class MedianFilter::Internal
{
    private:
        static std::map<ImageType, ComputeShader> computeShaders3x3; 
        static std::string shaderSrc3x3; 
        static std::map<ImageType, ComputeShader> computeShaders5x5; 
        static std::string shaderSrc5x5; 
        static bool shaderCompiled; 

        int size; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetSize(int s);
};

std::map<ImageType, ComputeShader> MedianFilter::Internal::computeShaders3x3;

std::string MedianFilter::Internal::shaderSrc3x3 = R"(

//3x3 median filter

layout( binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

#define s2(a, b)				temp = a; a = min(a, b); b = max(temp, b);
#define mn3(a, b, c)			s2(a, b); s2(a, c);
#define mx3(a, b, c)			s2(b, c); s2(a, c);

#define mnmx3(a, b, c)			mx3(a, b, c); s2(a, b);                                   // 3 exchanges
#define mnmx4(a, b, c, d)		s2(a, b); s2(c, d); s2(a, c); s2(b, d);                   // 4 exchanges
#define mnmx5(a, b, c, d, e)	s2(a, b); s2(c, d); mn3(a, c, e); mx3(b, d, e);           // 6 exchanges
#define mnmx6(a, b, c, d, e, f) s2(a, d); s2(b, e); s2(c, f); mn3(a, b, c); mx3(d, e, f); // 7 exchanges

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    vec3 v[9];

    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 orig = imageLoad(inputImage, id);

    for(int i = -1; i <= 1; i++)
    {
        for(int j = -1; j <= 1; j++) 
        {		
            ivec2 offset = ivec2(i, j);      
            v[(i + 1) * 3 + (j + 1)] = imageLoad(inputImage, id + offset).rgb;
        }
    }

    vec3 temp;

    // Starting with a subset of size 6, remove the min and max each time
    mnmx6(v[0], v[1], v[2], v[3], v[4], v[5]);
    mnmx5(v[1], v[2], v[3], v[4], v[6]);
    mnmx4(v[2], v[3], v[4], v[7]);
    mnmx3(v[3], v[4], v[8]);

    imageStore(outputImage, id, vec4(v[4], 1.0f)); 
}

)";

std::map<ImageType, ComputeShader> MedianFilter::Internal::computeShaders5x5;

std::string MedianFilter::Internal::shaderSrc5x5 = R"(

//5x5 median filter

layout(binding=0) writeonly uniform image2D outputImage;
layout(FORMAT_QUALIFIER, binding=1) uniform image2D inputImage;

#define s2(a, b)				temp = a; a = min(a, b); b = max(temp, b);
#define t2(a, b)				s2(v[a], v[b]);
#define t24(a, b, c, d, e, f, g, h)			t2(a, b); t2(c, d); t2(e, f); t2(g, h); 
#define t25(a, b, c, d, e, f, g, h, i, j)		t24(a, b, c, d, e, f, g, h); t2(i, j);

layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
void main()
{
    vec3 v[25];

    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    vec4 orig = imageLoad(inputImage, id);

    for(int i = -2; i <= 2; i++)
    {
        for(int j = -2; j <= 2; j++) 
        {		
            ivec2 offset = ivec2(i, j);      
            v[(i + 2) * 5 + (j + 2)] = imageLoad(inputImage, id + offset).rgb;
        }
    }

    vec3 temp;

    t25(0, 1,			3, 4,		2, 4,		2, 3,		6, 7);
    t25(5, 7,			5, 6,		9, 7,		1, 7,		1, 4);
    t25(12, 13,		11, 13,		11, 12,		15, 16,		14, 16);
    t25(14, 15,		18, 19,		17, 19,		17, 18,		21, 22);
    t25(20, 22,		20, 21,		23, 24,		2, 5,		3, 6);
    t25(0, 6,			0, 3,		4, 7,		1, 7,		1, 4);
    t25(11, 14,		8, 14,		8, 11,		12, 15,		9, 15);
    t25(9, 12,		13, 16,		10, 16,		10, 13,		20, 23);
    t25(17, 23,		17, 20,		21, 24,		18, 24,		18, 21);
    t25(19, 22,		8, 17,		9, 18,		0, 18,		0, 9);
    t25(10, 19,		1, 19,		1, 10,		11, 20,		2, 20);
    t25(2, 11,		12, 21,		3, 21,		3, 12,		13, 22);
    t25(4, 22,		4, 13,		14, 23,		5, 23,		5, 14);
    t25(15, 24,		6, 24,		6, 15,		7, 16,		7, 19);
    t25(3, 11,		5, 17,		11, 17,		9, 17,		4, 10);
    t25(6, 12,		7, 14,		4, 6,		4, 7,		12, 14);
    t25(10, 14,		6, 7,		10, 12,		6, 10,		6, 17);
    t25(12, 17,		7, 17,		7, 10,		12, 18,		7, 12);
    t24(10, 18,		12, 20,		10, 20,		10, 12);

    imageStore(outputImage, id, vec4(v[12], 1.0f)); 
}

)";

bool MedianFilter::Internal::shaderCompiled = false; 

MedianFilter::Internal::Internal()
{
    size = 3; 
}


void MedianFilter::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(!shaderCompiled)
    {
        CompileImageComputeShaders(computeShaders3x3, shaderSrc3x3); 
        CompileImageComputeShaders(computeShaders5x5, shaderSrc5x5); 
        shaderCompiled = true; 
    }

    ReallocateIfNotSameSize(output, input); 

    ImageType inputType = input->GetType();
    
    ComputeShader& computeShader = ComputeShader();
    if(size == 5)
        computeShader = computeShaders5x5[inputType];
    else
        computeShader = computeShaders3x3[inputType];

    computeShader.SetImage("inputImage", input);
    computeShader.SetImage("outputImage", output, ComputeShader::WRITE_ONLY);

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 
    computeShader.Dispatch(groupCount.x, groupCount.y, 1); 
    computeShader.Block();
}

void MedianFilter::Internal::Run(Image* input, Image* output)
{
    ReallocateIfNotSameSize(output, input); 
    
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

void MedianFilter::Internal::SetSize(int s)
{
    size = s; 
}



MedianFilter::MedianFilter()
{
    internal = new Internal(); 
}

MedianFilter::~MedianFilter()
{
    delete internal; 
}

void MedianFilter::SetSize(int s)
{
    internal->SetSize(s);
}

void MedianFilter::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void MedianFilter::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}