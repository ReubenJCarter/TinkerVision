#include "Resize.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"
#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Viso
{
namespace CompositeProcess
{

class Resize::Internal
{
    private:
        Process::Upsample::Mode upsampleMode; 
        Process::Downsample::Mode downsampleMode; 

       
    public:
        Internal(); 
        void Run(ImageGPU* input, ImageGPU* output);
        void Run(Image* input, Image* output);
        void SetUpsampleMode(Process::Upsample::Mode m);
        void SetDownsampleMode(Process::Downsample::Mode m); 
};


Resize::Internal::Internal()
{
    upsampleMode = Process::Upsample::Mode::BILINEAR; 
    downsampleMode = Process::Downsample::Mode::BOX; 

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

void Resize::Internal::SetUpsampleMode(Process::Upsample::Mode m)
{
    upsampleMode = m; 
}

void Resize::Internal::SetDownsampleMode(Process::Downsample::Mode m)
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

void Resize::SetUpsampleMode(Process::Upsample::Mode m)
{
    internal->SetUpsampleMode(m); 
}

void Resize::SetDownsampleMode(Process::Downsample::Mode m)
{
    internal->SetDownsampleMode(m); 
}

}
}