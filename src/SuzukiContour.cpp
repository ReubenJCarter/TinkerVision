#include "SuzukiContour.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class SuzukiContour::Internal
{
    private:

    public:
        Internal(); 
        void Run(Image* input, Image* output);
};


SuzukiContour::Internal::Internal()
{
}

void SuzukiContour::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE16) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE16); 
    }
    unsigned char* inputData = input->GetData(); 
    uint16_t* outputData = (uint16_t*)output->GetData(); 

    //copy threshold to output image
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            glm::vec4 pix = GetPixel(input, i, j); 
            if(pix.r > 0.5f)
            {
                outputData[j * input->GetWidth() + i] = 1; 
            }
            else
            {
                outputData[j * input->GetWidth() + i] = 0; 
            }
        }
    }

    int nbd = 1;
    int lnbd;
    int i2, j2;
    int i3, j3; 
    int i4, j4; 
    
    for(int j = 0; j < input->GetHeight(); j++)
    {
        lnbd = 1; 
        int prevPix = 0; 
        for(int i = 0; i < input->GetWidth(); i++)
        {
            //For Each Pixel
            int pix = GetPixelI(output, i, j).r;
            int pixNext = GetPixelI(output, i+1, j).r;

            //Section 1; 
            int state;
            if(pix == 1 && prevPix ==0)
            {
                //If fij = 1 and fi, j-1 = 0, then decide that the pixel (i, j) is the border
                //following starting point of an outer border, increment NBD, and (i2, j2) <- (i, j - 1).
                i2 = i;
                j2 = j - 1; 

                nbd++;

                state = 0;
            }
            else if(pix >= 1 && pixNext == 0)
            {
                //Else if fij >= 1 and fi,j+l = 0, then decide that the pixel (i, j) is the
                //border following starting point of a hole border, increment NBD, (iz, j,) * (i, j + l)
                //, and LNBD <- fij in case fij > 1.
                i2 = i;
                j2 = j + 1; 

                nbd++; 
                if(pix > 1)
                    lnbd = pix;

                state = 1; 
            }
            else
            {
                state = 2; 
            }

            //Section 2
            


         
            prevPix = pix; 
        } 
    } 
}




SuzukiContour::SuzukiContour()
{
    internal = new Internal(); 
}

SuzukiContour::~SuzukiContour()
{ 
    delete internal; 
}

void SuzukiContour::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}