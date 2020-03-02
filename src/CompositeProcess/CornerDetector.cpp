#include "CornerDetector.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Process/GrayScale.h"
#include "../Process/GaussianBlur.h"
#include "../Process/Sobel.h"
#include "../Process/NonMaximumEdgeSuppression.h"
#include "../Process/StructureTensor.h"
#include "../Process/HarrisShiTomasiResponse.h"


#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{
namespace CompositeProcess
{

using namespace Process;

//http://www.ipol.im/pub/art/2018/229/article_lr.pdf

class CornerDetector::Internal
{
    private:
        GrayScale grayscale; 
        GaussianBlur gaussianBlur; 
        Sobel sobel; 
        NonMaximumEdgeSuppression nms; 
        StructureTensor strucTens;
        HarrisShiTomasiResponse harShiTomResp;

        ImageGPU temp[4]; 

        float sigmaD; 
        float sigmaI; 
        float harrisK; 
        float shiTomasiThreshold;

    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);

        void SetSigmaI(float sig); 
        void SetSigmaD(float sig); 
        void SetK(float kk); 
        void SetHarrisK(float kk);
        void SetShiTomasiThreshold(float t);
};

CornerDetector::Internal::Internal()
{
    sigmaI = 2; 
    sigmaD = 1; 
    harrisK = 0.05;
    shiTomasiThreshold = 0.001;
}

void CornerDetector::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::RGBA32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::RGBA32F); 
    }

    glm::ivec2 groupCount = ComputeWorkGroupCount(glm::ivec2(input->GetWidth(), input->GetHeight()), glm::i32vec2(16, 16)); 

    //Grayscale if needed ( color in, gray output)
    if( !(input->GetType() == ImageType::GRAYSCALE8 || input->GetType() == ImageType::GRAYSCALE16 || input->GetType() == ImageType::GRAYSCALE32F ) )
    {
        grayscale.Run(input, &temp[0] ); 
        input = &temp[0]; 
    } 

    //Gaussian Blur ( gray in, gray output)
    gaussianBlur.SetSigma(sigmaD); 
    gaussianBlur.Run(input, &temp[1]);

    //Sobel Gradients ( gray in, 4xfloat output)
    sobel.Run(&temp[1], &temp[2]); 

    //Compute Ix^2 IxIy Iy^2
    strucTens.Run(&temp[2], &temp[3]);

    //Gaussian Blur ( gray in, gray output)
    gaussianBlur.SetSigma(sigmaI); 
    gaussianBlur.Run(&temp[3], &temp[2]);

    //Compute Harris response
    harShiTomResp.Run(&temp[2], output);

}

void CornerDetector::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || 
       output->GetHeight() != input->GetHeight() || 
       output->GetType() != ImageType::GRAYSCALE32F)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE32F); 
    }
    
    unsigned char* inputData = input->GetData(); 
    unsigned char* outputData = output->GetData(); 
    
}

void CornerDetector::Internal::SetSigmaI(float sig)
{
    sigmaI = sig; 
}

void CornerDetector::Internal::SetSigmaD(float sig)
{
    sigmaD = sig; 
}

void CornerDetector::Internal::SetHarrisK(float kk)
{
    harrisK = kk;
}

void CornerDetector::Internal::SetShiTomasiThreshold(float t)
{
    shiTomasiThreshold = t;
}




CornerDetector::CornerDetector()
{
    internal = new Internal(); 
}

CornerDetector::~CornerDetector()
{
    delete internal; 
}

void CornerDetector::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void CornerDetector::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}