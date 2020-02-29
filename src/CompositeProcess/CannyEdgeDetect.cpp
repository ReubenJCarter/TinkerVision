#include "CannyEdgeDetect.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Process/GaussianBlur.h"
#include "../Process/Sobel.h"
#include "../Process/NonMaximumEdgeSuppression.h"
#include "../Process/HysteresisEdgeThreshold.h"
#include "../Process/GrayScale.h"
#include "../Process/ChannelDemux.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class CannyEdgeDetect::Internal
{
    private:
        ImageGPU tempGPU[4]; 
        Image temp;
        GrayScale gray; 
        GaussianBlur blur; 
        Sobel sobel; 
        NonMaximumEdgeSuppression nms;
        HysteresisEdgeThreshold threshold; 

        float sigma;
        float highEdgeThreshold;
        float lowEdgeThreshold; 

    public:
        Internal(); 
        void Run(ImageGPU* input, Image* output);

        void RunGray(ImageGPU* input, ImageGPU* output); 
        void RunBlur(ImageGPU* input, ImageGPU* output); 
        void RunSobel(ImageGPU* input, ImageGPU* output); 
        void RunNMS(ImageGPU* input, ImageGPU* output); 
        void RunThresholding(Image* input, Image* output); 

        void SetBlurSigma(float sigm); 
        void SetHighEdgeThreshold(float t);
        void SetLowEdgeThreshold(float t);

        
};


CannyEdgeDetect::Internal::Internal()
{
    sigma = 3; 
    highEdgeThreshold = 0.8; 
    lowEdgeThreshold = 0.2; 
}

void CannyEdgeDetect::Internal::Run(ImageGPU* input, Image* output)
{
    bool grayConverted = false; 
    if(input->GetType() != ImageType::GRAYSCALE8 && input->GetType() != ImageType::GRAYSCALE16 && input->GetType() != ImageType::GRAYSCALE32F)
    {
        RunGray(input, &tempGPU[0]); 
        input = &tempGPU[0]; 
    }
    if(sigma != 0)
    {
        tempGPU[1].Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
        RunBlur(input, &tempGPU[1]); 
        RunSobel(&tempGPU[1], &tempGPU[2]); 
    }
    else
    {
        tempGPU[1].Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
        RunSobel(&tempGPU[0], &tempGPU[2]); 
    }

    RunNMS(&tempGPU[2], &tempGPU[3]); 

    Visi::ChannelDemux demux; 
    demux.SetChannel(2);
    demux.Run(&tempGPU[3], &tempGPU[1]);

    temp.Copy(&tempGPU[1]); 

    RunThresholding(&temp, output); 
    
    //output->Copy(&tempGPU[1]); 
}


void CannyEdgeDetect::Internal::RunGray(ImageGPU* input, ImageGPU* output)
{
    gray.Run(input, output); 
}

void CannyEdgeDetect::Internal::RunBlur(ImageGPU* input, ImageGPU* output)
{
    blur.SetSigma(sigma); 
    blur.Run(input, output); 
}

void CannyEdgeDetect::Internal::RunSobel(ImageGPU* input, ImageGPU* output)
{
    sobel.Run(input, output); 
}

void CannyEdgeDetect::Internal::RunNMS(ImageGPU* input, ImageGPU* output)
{
    nms.Run(input, output); 
}

void CannyEdgeDetect::Internal::RunThresholding(Image* input, Image* output)
{
    threshold.SetLowThreshold(lowEdgeThreshold);
    threshold.SetHighThreshold(highEdgeThreshold); 
    threshold.Run(input, output); 
}


void CannyEdgeDetect::Internal::SetBlurSigma(float sigm)
{
    sigma = sigm;
}

void CannyEdgeDetect::Internal::SetHighEdgeThreshold(float t)
{
    highEdgeThreshold = t;
}

void CannyEdgeDetect::Internal::SetLowEdgeThreshold(float t)
{
    lowEdgeThreshold = t;
}






CannyEdgeDetect::CannyEdgeDetect()
{
    internal = new Internal(); 
}

CannyEdgeDetect::~CannyEdgeDetect()
{ 
    delete internal; 
}


void CannyEdgeDetect::SetBlurSigma(float sigm)
{
    internal->SetBlurSigma(sigm); 
}

void CannyEdgeDetect::SetHighEdgeThreshold(float t)
{
    internal->SetHighEdgeThreshold(t); 
}

void CannyEdgeDetect::SetLowEdgeThreshold(float t)
{
    internal->SetLowEdgeThreshold(t); 
}


void CannyEdgeDetect::Run(ImageGPU* input, Image* output)
{
    internal->Run(input, output); 
}

void CannyEdgeDetect::RunGray(ImageGPU* input, ImageGPU* output)
{
    internal->RunGray(input, output);
}

void CannyEdgeDetect::RunBlur(ImageGPU* input, ImageGPU* output)
{
    internal->RunBlur(input, output);
}

void CannyEdgeDetect::RunSobel(ImageGPU* input, ImageGPU* output)
{
    internal->RunSobel(input, output); 
}

void CannyEdgeDetect::RunNMS(ImageGPU* input, ImageGPU* output)
{
    internal->RunNMS(input, output); 
}

void CannyEdgeDetect::RunThresholding(Image* input, Image* output)
{
    internal->RunThresholding(input, output); 
}

}