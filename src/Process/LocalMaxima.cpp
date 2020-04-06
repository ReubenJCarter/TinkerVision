#include "LocalMaxima.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>
#include <vector>

namespace Visi
{
namespace Process
{

class LocalMaxima::Internal
{
    private:
        float threshold;
        int size; 

    public:
        Internal(); 
        void Run(Image* input, Image* output, std::vector<Vec2>& localMaxima);
        void SetThreshold(float t); 
        void SetSize(int s);
};


LocalMaxima::Internal::Internal()
{
    size = 3;
    threshold = 0.00001f; 
}

void LocalMaxima::Internal::Run(Image* input, Image* output, std::vector<Vec2>& localMaxima)
{
    
    ReallocateIfNotSameSize(output, input,  ImageType::GRAYSCALE8); 

    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            SetPixelUI(output, i, j, glm::ivec4(0, 0, 0, 0) ); 
        }
    }
    
    localMaxima.clear(); 
    localMaxima.reserve(input->GetWidth() * input->GetHeight() * 0.2); 

    //Find local maximum points and zero output
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            glm::vec4 midPixel = GetPixel(input, i,   j); 

            //Test greated than threshold
            if(midPixel.r <= threshold)
                continue; 
            
            //test on neighbour (greatest in neighboorhood)
            bool isMax = true; 
            for(int x = -size/2; x <= size/2; x++)
            {
                for(int y = -size/2; y <= size/2; y++)
                {
                    if(x == 0 && y == 0)
                        continue; 
                    glm::vec4 pix = GetPixel(input, i + x, j + y); 
                    if(pix.r > midPixel.r)
                    {
                        isMax = false;
                        break; 
                    }
                }

                if(!isMax)
                    break;
            }

            if(isMax)
            {
                localMaxima.push_back( Vec2(i, j) ); 
                SetPixel(output, i, j, glm::vec4(1, 1, 1, 1) ); 
            }
        } 
    } 
}

void LocalMaxima::Internal::SetThreshold(float t)
{
    threshold = t;
}

void LocalMaxima::Internal::SetSize(int s)
{
    size = s;
}  



LocalMaxima::LocalMaxima()
{
    internal = new Internal(); 
}

LocalMaxima::~LocalMaxima()
{
    delete internal; 
}

void LocalMaxima::Run(Image* input, Image* output, std::vector<Vec2>& localMaxima)
{
    internal->Run(input, output, localMaxima); 
}

void LocalMaxima::SetThreshold(float t)
{
    internal->SetThreshold(t); 
}

void LocalMaxima::SetSize(int s)
{
    internal->SetSize(s); 
}

}
}