#include "HysteresisEdgeThreshold.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class HysteresisEdgeThreshold::Internal
{
    private:
        float highThreshold; 
        float lowThreshold; 

    public:
        Internal(); 
        void Run(Image* input, Image* output);
        void SetHighThreshold(float highThr);
        void SetLowThreshold(float lowThr);

};

HysteresisEdgeThreshold::Internal::Internal()
{
}

void HysteresisEdgeThreshold::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
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

void HysteresisEdgeThreshold::Internal::SetHighThreshold(float highThr)
{
    highThreshold = highThr;
}

void HysteresisEdgeThreshold::Internal::SetLowThreshold(float lowThr)
{
    lowThreshold = lowThr;
}



HysteresisEdgeThreshold::HysteresisEdgeThreshold()
{
    internal = new Internal(); 
}

HysteresisEdgeThreshold::~HysteresisEdgeThreshold()
{
    delete internal; 
}

void HysteresisEdgeThreshold::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void HysteresisEdgeThreshold::SetHighThreshold(float highThr)
{
    internal->SetHighThreshold(highThr);
}

void HysteresisEdgeThreshold::SetLowThreshold(float lowThr)
{
    internal->SetLowThreshold(lowThr); 
}   

}