#include "MorphThinning.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <functional>
#include <glm/glm.hpp>

namespace TnkrVis
{
namespace Process
{

class MorphThinning::Internal
{
    private:
        Image temp; 

    public:
        Internal(); 
        void Run(Image* input, Image* output);
        void CopyImage(Image* input, Image* output); 
        inline bool Thin(Image* input, Image* output, std::vector<glm::ivec2>& whitePixels, std::vector<glm::ivec2>& blackPixels)
        {
            CopyImage(input, output); 
            bool anyMatch = false;
            for(int j = 0; j < input->GetHeight(); j++)
            {
                for(int i = 0; i < input->GetWidth(); i++)
                {

                    bool match = true;
                    for(int k = 0; k < whitePixels.size(); k++)
                    {
                        glm::vec4 pix = GetPixel(input, i + whitePixels[k].x , j + whitePixels[k].y);
                        if(pix.r < 0.5)
                        {
                            match = false; 
                            break; 
                        }
                    }
                    
                    if(!match)
                        continue; 

                    for(int k = 0; k < blackPixels.size(); k++)
                    {
                        glm::vec4 pix = GetPixel(input, i + blackPixels[k].x , j + blackPixels[k].y);
                        if(pix.r > 0.5)
                        {
                            match = false; 
                            break; 
                        }
                    }

                    if(!match)
                        continue; 

                    //supress pixel
                    glm::vec4 outPix = glm::vec4(0, 0, 0, 1); 
                    SetPixel(output, i, j, outPix); 
                    anyMatch = true; 
                }
            }
            return anyMatch; 
        }
};


MorphThinning::Internal::Internal()
{
}

void MorphThinning::Internal::CopyImage(Image* input, Image* output)
{
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            glm::vec4 pix = GetPixel(input, i, j); 
            SetPixel(output, i, j, pix); 
        }
    }
}

void MorphThinning::Internal::Run(Image* input, Image* output)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE8) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE8); 
        temp.Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE8); 
    }

    std::vector<glm::ivec2> whites;
    std::vector<glm::ivec2> blacks;
    

    for(int i = 0; i < 10; i++)
    {
        bool change = false; 
        bool c; 

        whites = {{0, 0},   {-1, -1}, {0, -1}, {1, -1}}; 
        blacks = {{-1, 1}, {0, 1}, {1, 1}};
        c = Thin(input, &temp, whites, blacks); 
        change = c ? true : change; 
        whites = {{-1, 0}, {0, 0}, {0, -1}}; 
        blacks = {{0, 1}, {1, 1}, {1, 0}};
        c = Thin(&temp, output, whites, blacks); 
        change = c ? true : change; 

        //90 degree rotate clockwise
        whites = {{0, 0},   {-1, 1}, {-1, 0}, {-1, -1}}; 
        blacks = {{1, 1}, {1, 0}, {1, -1}};
        c = Thin(output, &temp, whites, blacks); 
        change = c ? true : change; 
        whites = {{-1, 0}, {0, 0}, {0, 1}}; 
        blacks = {{0, -1}, {1, -1}, {1, 0}};
        c = Thin(&temp, output, whites, blacks); 
        change = c ? true : change; 

        //180 degree rotate clockwise
        whites = {{0, 0},   {-1, 1}, {0, 1}, {1, 1}}; 
        blacks = {{-1, -1}, {0, -1}, {1, -1}};
        c = Thin(output, &temp, whites, blacks); 
        change = c ? true : change; 
        whites = {{0, 1}, {0, 0}, {1, 0}}; 
        blacks = {{-1, 0}, {-1, -1}, {0, -1}};
        c = Thin(&temp, output, whites, blacks); 
        change = c ? true : change; 

        //270 degree rotate clockwise
        whites = {{0, 0},   {1, 1}, {1, 0}, {1, -1}}; 
        blacks = {{-1, 1}, {-1, 0}, {-1, -1}};
        c = Thin(output, &temp, whites, blacks); 
        change = c ? true : change; 
        whites = {{1, 0}, {0, 0}, {0, -1}}; 
        blacks = {{0, 1}, {-1, 1}, {-1, 0}};
        c = Thin(&temp, output, whites, blacks); 
        change = c ? true : change; 

        if(!change)
            break; 

        input = output; 
    }
}




MorphThinning::MorphThinning()
{
    internal = new Internal(); 
}

MorphThinning::~MorphThinning()
{ 
    delete internal; 
}

void MorphThinning::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}


}
}