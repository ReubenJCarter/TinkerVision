#include "Sift.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include "../Process/GaussianBlur.h"
#include "../Process/Downsample.h"
#include "../Process/CopyImage.h"

namespace Visi
{
namespace CompositeProcess
{

class Sift::Internal
{
    private:
        Process::GaussianBlur guassianBlur; 
        Process::Downsample downsample; 
        Process::CopyImage copyImage; 

        std::vector<ImageGPU> pyramid; 

        float sigma;
        int octaves;
        int scales; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        std::vector<ImageGPU>* GetPyramid();
};


Sift::Internal::Internal()
{
    sigma = 1.6f; 
    octaves = 4;
    scales = 5; 
    copyImage.SetFormatTranslate(false); 
    downsample.SetMode(Visi::Process::Downsample::BOX); 
}

void Sift::Internal::Run(ImageGPU* input, ImageGPU* output)
{

    //With guidance from https://medium.com/@lerner98/implementing-sift-in-python-36c619df7945
    pyramid.resize(octaves*(scales+3)); 
    for(int i = 0; i < octaves; i++)
    {
        for(int j = 0; j < scales + 3; j++) //we need to create 3 extra scales (this is so there is padding to create DOG and extrema )
        {
            ImageGPU* dstIm = &(pyramid[i * (scales+3) + j]);
            if(j == 0) //need to copy/resize the prev octave, or the input image as this is the start of an octave 
            {  
                if(i == 0)//start of whole process 
                {
                    //get image from input
                    copyImage.Run(input, dstIm);
                }
                else//start of new octave
                {
                    ImageGPU* srcIm = &(pyramid[i * (scales+3) + j - 3]); // choose the last image of the previous octave?? No should be third from last(remove padding)!
                    dstIm->Allocate(srcIm->GetWidth()/2, srcIm->GetHeight()/2, srcIm->GetType());
                    downsample.Run(srcIm, dstIm); 
                }
            }
            else //scale, blur preveious image 
            {
                ImageGPU* srcIm = &(pyramid[i * (scales+3) + j - 1]);
                guassianBlur.Run(srcIm, dstIm); 
            }
        }
    }
}

void Sift::Internal::Run(Image* input, Image* output)
{
    
}

std::vector<ImageGPU>* Sift::Internal::GetPyramid()
{
    return &pyramid;
}


Sift::Sift()
{
    internal = new Internal(); 
}

Sift::~Sift()
{ 
    delete internal; 
}

void Sift::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Sift::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

std::vector<ImageGPU>* Sift::GetPyramid()
{
   return internal->GetPyramid(); 
}


}
}