#include "Normalize.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class Normalize::Internal
{
    private:


    public:
        Internal(); 
        void Run(Image* input, Image* output);
};

Normalize::Internal::Internal()
{
}

void Normalize::Internal::Run(Image* input, Image* output)
{
    
    ReallocateIfNotSameSize(output, input); 
    
    glm::vec4 minI;
    glm::vec4 maxI; 

    if(input->GetWidth() > 0 && input->GetHeight() > 0)
    {
        minI = GetPixel(input, 0, 0);
        maxI = minI;
    }

    for(int i = 0; i < input->GetHeight(); i++)
    {
        for(int j = 0; j < input->GetWidth(); j++)
        {
            glm::vec4 d = GetPixel(input, j, i);
            if(d.r < minI.r) 
                minI.r = d.r;
            if(d.g < minI.g) 
                minI.g = d.g;
            if(d.b < minI.b) 
                minI.b = d.b;
            if(d.a < minI.a) 
                minI.a = d.a;

            if(d.r > maxI.r) 
                maxI.r = d.r;
            if(d.g > maxI.g) 
                minI.g = d.g;
            if(d.b > maxI.b) 
                maxI.b = d.b;
            if(d.a > maxI.a) 
                maxI.a = d.a;
        } 
    }

    for(int i = 0; i < input->GetHeight(); i++)
    {
        for(int j = 0; j < input->GetWidth(); j++)
        {
            glm::vec4 d = GetPixel(input, j, i);
            d = ( (d - minI) * (glm::vec4(1) - glm::vec4(0)) ) / (maxI - minI) + glm::vec4(0);
            SetPixel(output, j, i, d); 
        }
    }
}



Normalize::Normalize()
{
    internal = new Internal(); 
}

Normalize::~Normalize()
{
    delete internal; 
}

void Normalize::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}
}