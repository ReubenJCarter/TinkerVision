#include "Resize.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class Resize::Internal
{
    private:
        Upsample::Mode upsampleMode; 
        Downsample::Mode downsampleMode; 
       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetUpsampleMode(Upsample::Mode m);
        void SetDownsampleMode(Downsample::Mode m); 
};


Resize::Internal::Internal()
{
    upsampleMode = Upsample::Mode::BILINEAR; 
    downsampleMode = Downsample::Mode::BOX; 
}

void Resize::Internal::Run(ImageGPU* input, ImageGPU* output)
{
    
    if(output->GetType() != input->GetType()) 
    {
        output->Allocate(output->GetWidth(), output->GetHeight(), input->GetType()); 
    }

    

}

void Resize::Internal::Run(Image* input, Image* output)
{
    
    
}

void Resize::Internal::SetUpsampleMode(Upsample::Mode m)
{
    upsampleMode = m; 
}

void Resize::Internal::SetDownsampleMode(Downsample::Mode m)
{
    downsampleMode = m; 
}



Resize::Resize()
{
    internal = new Internal(); 
}

Resize::~Resize()
{ 
    delete internal; 
}

void Resize::Run(ImageGPU* input, ImageGPU* output)
{
    internal->Run(input, output); 
}

void Resize::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Resize::SetUpsampleMode(Upsample::Mode m)
{
    internal->SetUpsampleMode(m); 
}

void Resize::SetDownsampleMode(Downsample::Mode m)
{
    internal->SetDownsampleMode(m); 
}

}