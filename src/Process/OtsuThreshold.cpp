#include "OtsuThreshold.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Viso
{
namespace Process
{

class OtsuThreshold::Internal
{
    private:
       
    public:
        Internal(); 
        float Run(Image* input);
};

OtsuThreshold::Internal::Internal()
{
}

//Information from here: http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html
float OtsuThreshold::Internal::Run(Image* input)
{
    //build histogram
    std::vector<int> hist(256); 
    for(int i = 0; i < 256; i++)
    {
        hist[i] = 0; 
    }
    
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            glm::vec4 pix = GetPixel(input, i, j); 
            int r = pix.x * 255; 
            if(r > 255) r = 255;
            if(r < 0) r = 0; 
            hist[r]++; 
        }
    }

    //Sum of the pixels
    double pixelSum = 0; //this will get very big !
    for(int i =0; i < 256; i++)
    {
        pixelSum += i * hist[i]; 
    }
    
    //Compute the actuyal threshold
    double sumB = 0;
    int wB = 0;
    int wF = 0;

    double varMax = 0;
    int threshold = 0;

    int total = input->GetWidth() * input->GetHeight(); 

    for (int t = 0; t < 256; t++) 
    {
        wB += hist[t];// Weight Background
        if (wB == 0) continue;

        wF = total - wB; // Weight Foreground
        if (wF == 0) break;

        sumB += (double) (t * hist[t]);

        double mB = sumB / wB;// Mean Background
        double mF = (pixelSum - sumB) / wF;// Mean Foreground

        // Calculate Between Class Variance
        double varBetween = (double)wB * (double)wF * (mB - mF) * (mB - mF);

        // Check if new maximum found
        if (varBetween > varMax) 
        {
            varMax = varBetween;
            threshold = t;
        }
    }

    return (double)threshold / 255.0f;
}



OtsuThreshold::OtsuThreshold()
{
    internal = new Internal(); 
}

OtsuThreshold::~OtsuThreshold()
{
    delete internal; 
}

float OtsuThreshold::Run(Image* input)
{
    return internal->Run(input); 
}

}
}