#include "LocalMaxima.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>
#include <vector>

namespace Visi
{

class LocalMaxima::Internal
{
    private:

    public:
        Internal(); 
        void Run(Image* input, Image* output);
};


LocalMaxima::Internal::Internal()
{
}

void LocalMaxima::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE8)
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE8); 
    }

    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            SetPixel(output, i, j, glm::vec4(0, 0, 0, 0) ); 
        }
    }

    //Find local maximum points and zero output
    std::vector<glm::ivec2> localMaxima; 
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

            if(pix4.r >= pix0.r &&
               pix4.r >= pix1.r &&
               pix4.r >= pix2.r &&
               pix4.r >= pix3.r &&
               pix4.r >= pix5.r &&
               pix4.r >= pix6.r &&
               pix4.r >= pix7.r &&
               pix4.r >= pix8.r )
            {
                localMaxima.push_back( glm::ivec2(i, j) ); 
                SetPixel(output, i, j, glm::vec4(1, 1, 1, 1) ); 
            }

            
        } 
    } 
}



LocalMaxima::LocalMaxima()
{
    internal = new Internal(); 
}

LocalMaxima::~LocalMaxima()
{
    delete internal; 
}

void LocalMaxima::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

}