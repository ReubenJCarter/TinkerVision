#include "ApproxDistanceTransform.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#define MIN(x, y) ((x)<(y)?(x):(y))

#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <functional>

#include "Sobel.h"
#include "ChannelDemux.h"

namespace Visi
{

class ApproxDistanceTransform::Internal
{
    private:
        Image temp; 
        Sobel sobel;
        ChannelDemux demux; 

    public:
        Internal(); 
        void Run(Image* input, Image* output);
};


ApproxDistanceTransform::Internal::Internal()
{
}

void ApproxDistanceTransform::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE16) 
    {
        //temp.Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE16); 
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE16); 
    }
    uint16_t* outputData = (uint16_t*)output->GetData(); 

    //copy threshold to output image
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            int inx = j * input->GetWidth() + i; 
            glm::vec4 pix = GetPixel(input, i, j); 
            if(pix.r > 0.5f)
            {
                outputData[inx] = 1; 
            }
            else
            {
                outputData[inx] = 0; 
            }
            SetPixelUI(&temp, i, j, glm::vec4(0, 0, 0, 0)); 
        }
    }

    //Edge thinning
    //https://www.researchgate.net/publication/221016857_An_Efficient_Euclidean_Distance_Transform
    //sweep 1
    int a = 1; 
    int b = 1; 
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            int inx = j * input->GetWidth() + i; 
            if( outputData[inx] == 0)
                continue; 
            
            int pixel0 = GetPixelUI(output, i-1, j).r + a; 
            int pixel1 = GetPixelUI(output, i-1, j-1).r + b; 
            int pixel2 = GetPixelUI(output, i, j-1).r + a; 
            int pixel3 = GetPixelUI(output, i+1, j-1).r + b; 
            int min01 = MIN(pixel0, pixel1); 
            int min23 = MIN(pixel2, pixel3); 
            int minAll = MIN(min01, min23); 
            outputData[inx] = minAll;

            SetPixel(&temp, i, j, glm::vec4() ); 
        }
    }
    //sweep 2
    for(int j = input->GetHeight()-1; j >= 0; j--)
    {
        for(int i = input->GetWidth()-1; i >= 0; i--)
        {
            int inx = j * input->GetWidth() + i; 
            if( outputData[inx] == 0)
                continue; 
            
            int pixel0 = GetPixelUI(output, i, j).r; 
            int pixel1 = GetPixelUI(output, i+1, j).r + a; 
            int pixel2 = GetPixelUI(output, i-1, j+1).r + b; 
            int pixel3 = GetPixelUI(output, i, j+1).r + a; 
            int pixel4 = GetPixelUI(output, i+1, j+1).r + b; 
            int min12 = MIN(pixel1, pixel2); 
            int min34 = MIN(pixel3, pixel4); 
            int minAll = MIN(min12, min34); 
            outputData[inx] = MIN(minAll, pixel0);
        }
    }
    
}




ApproxDistanceTransform::ApproxDistanceTransform()
{
    internal = new Internal(); 
}

ApproxDistanceTransform::~ApproxDistanceTransform()
{ 
    delete internal; 
}

void ApproxDistanceTransform::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}


}