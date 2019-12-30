#include "Thinning.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <functional>

namespace Visi
{

class Thinning::Internal
{
    private:
        Image temp; 

    public:
        Internal(); 
        void Run(Image* input, Image* output);
};


Thinning::Internal::Internal()
{
}

void Thinning::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE16) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE16); 
    }
    uint16_t* outputData = (uint16_t*)output->GetData(); 

    //copy threshold to output image
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            glm::vec4 pix0 = GetPixel(input, i-1, j-1);
            glm::vec4 pix1 = GetPixel(input, i,   j-1);
            glm::vec4 pix2 = GetPixel(input, i+1, j-1);
            glm::vec4 pix3 = GetPixel(input, i-1, j);
            glm::vec4 pix4 = GetPixel(input, i,   j);
            glm::vec4 pix5 = GetPixel(input, i+1, j);
            glm::vec4 pix6 = GetPixel(input, i-1, j+1);
            glm::vec4 pix7 = GetPixel(input, i,   j+1);
            glm::vec4 pix8 = GetPixel(input, i+1, j+1);
            
            glm::vec4 outPix; 
            SetPixelUI(output, i, j, outPix); 
        }
    }
}




Thinning::Thinning()
{
    internal = new Internal(); 
}

Thinning::~Thinning()
{ 
    delete internal; 
}

void Thinning::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}


}