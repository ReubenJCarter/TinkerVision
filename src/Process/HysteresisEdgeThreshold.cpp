#include "HysteresisEdgeThreshold.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <map>

#include <vector> 

namespace Viso
{
namespace Process
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
        void TraceBorder(Image* inputImage, Image* outputImage, int startI, int startJ, std::vector<glm::ivec2>& countour);
};

HysteresisEdgeThreshold::Internal::Internal()
{
    highThreshold = 0.5;
    lowThreshold = 0.1; 
}

//Using Theo Pavlidis algorithm
void HysteresisEdgeThreshold::Internal::TraceBorder(Image* inputImage, Image* outputImage, int startI, int startJ, std::vector<glm::ivec2>& countour)
{
    static enum DIRECTION{N, NE, E, SE, S, SW, W, NW};
    
    int ladybirdI = startI;
    int ladybirdJ = startJ; 
    int startCounter = 0;
    int rotateCounter = 0; 
    DIRECTION direction = N;
    int p1, p2, p3;
    int pixelValue = 3; 

    bool contourContainsHighThresh = false; 

    while(startCounter < 2 && rotateCounter <= 3)
    {
        if(direction == N)
        {
            p1 = GetPixelUI(outputImage, ladybirdI - 1, ladybirdJ + 1).r;  
            p2 = GetPixelUI(outputImage, ladybirdI,     ladybirdJ + 1).r;  
            p3 = GetPixelUI(outputImage, ladybirdI + 1, ladybirdJ + 1).r;
            if(p1 > 0)
            {
                pixelValue = p1; 
                ladybirdI -= 1;
                ladybirdJ += 1; 
                direction = W;    
                rotateCounter = 0;                            
            }  
            else if(p2 > 0)
            {
                pixelValue = p2;
                ladybirdJ += 1; 
                direction = N;  
                rotateCounter = 0; 
            }
            else if(p3 > 0)
            {
                pixelValue = p3;
                ladybirdI += 1; 
                ladybirdJ += 1;
                direction = N;  
                rotateCounter = 0; 
            }
            else 
            {
                direction = E;
                rotateCounter++;  
            }
        }
        else if(direction == S)
        {
            p1 = GetPixelUI(outputImage, ladybirdI + 1, ladybirdJ - 1).r;  
            p2 = GetPixelUI(outputImage, ladybirdI,     ladybirdJ - 1).r;  
            p3 = GetPixelUI(outputImage, ladybirdI - 1, ladybirdJ - 1).r;  

            if(p1 > 0)
            {
                pixelValue = p1;
                ladybirdI += 1;
                ladybirdJ -= 1; 
                direction = E;   
                rotateCounter = 0;                              
            }  
            else if(p2 > 0)
            {
                pixelValue = p2;
                ladybirdJ -= 1; 
                direction = S;  
                rotateCounter = 0; 
            }
            else if(p3 > 0)
            {
                pixelValue = p3;
                ladybirdI -= 1; 
                ladybirdJ -= 1;
                direction = S;  
                rotateCounter = 0; 
            }
            else 
            {
                direction = W;  
                rotateCounter++;
            }
        }
        else if(direction == E)
        {
            p1 = GetPixelUI(outputImage, ladybirdI + 1, ladybirdJ + 1).r;  
            p2 = GetPixelUI(outputImage, ladybirdI + 1, ladybirdJ).r;  
            p3 = GetPixelUI(outputImage, ladybirdI + 1, ladybirdJ - 1).r;  

            if(p1 > 0)
            {
                pixelValue = p1;
                ladybirdI += 1;
                ladybirdJ += 1; 
                direction = N;    
                rotateCounter = 0;                            
            }  
            else if(p2 > 0)
            {
                pixelValue = p2;
                ladybirdI += 1; 
                direction = E;  
                rotateCounter = 0; 
            }
            else if(p3 > 0)
            {
                pixelValue = p3;
                ladybirdI += 1; 
                ladybirdJ -= 1;
                direction = E;  
                rotateCounter = 0;
            }
            else 
            {
                direction = S;  
                rotateCounter++; 
            }
        }
        else if(direction == W)
        {
            p1 = GetPixelUI(outputImage, ladybirdI - 1, ladybirdJ - 1).r;  
            p2 = GetPixelUI(outputImage, ladybirdI - 1, ladybirdJ).r;  
            p3 = GetPixelUI(outputImage, ladybirdI - 1, ladybirdJ + 1).r;  

            if(p1 > 0)
            {
                pixelValue = p1;
                ladybirdI -= 1;
                ladybirdJ -= 1; 
                direction = S;      
                rotateCounter = 0;                          
            }  
            else if(p2 > 0)
            {
                pixelValue = p2;
                ladybirdI -= 1; 
                direction = W;  
                rotateCounter = 0;
            }
            else if(p3 > 0)
            {
                pixelValue = p3;
                ladybirdI -= 1; 
                ladybirdJ += 1;
                direction = W;  
                rotateCounter = 0; 
            }
            else 
            {
                direction = N;  
                rotateCounter++; 
            }
        }

        if(ladybirdI == startI && ladybirdJ == startJ && rotateCounter == 0)
            startCounter++; 
        
        if(pixelValue == 2)
        {
            contourContainsHighThresh = true; 
        }

        if(pixelValue <= 2)//new ground
        {
            SetPixelUI(outputImage, ladybirdI, ladybirdJ, glm::ivec4(3, 0, 0, 1) ); 
            countour.push_back(glm::ivec2(ladybirdI, ladybirdJ)); 
        }
        //std::cout << "TRACEPIXELSET " << ladybirdI << " " << ladybirdJ << " " << startCounter << " " << rotateCounter << "\n";
    }
    
    //draw contour
    if(contourContainsHighThresh)
    {
        for(int i = 0; i < countour.size(); i++)
        {
            int X = countour[i].x;
            int Y = countour[i].y;
            SetPixelUI(outputImage, X, Y, glm::ivec4(4, 0, 0, 1) ); 
        }
    }
}

void HysteresisEdgeThreshold::Internal::Run(Image* input, Image* output)
{

    ReallocateIfNotSameSize(output, input, ImageType::GRAYSCALE16); 
    
    uint16_t* outputData = (uint16_t*)output->GetData(); 

    //copy threshold to output image
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            glm::vec4 pix = GetPixel(input, i, j); 
            
            int inx = j * output->GetWidth() + i; 
            if(pix.r > highThreshold)
            {
                //outputData[inx] = 2;
                SetPixelUI(output, i, j, glm::ivec4(2, 0, 0, 1) ); 
            }
            else if(pix.r > lowThreshold)
            {
                 //outputData[inx] = 1;
                 SetPixelUI(output, i, j, glm::ivec4(1, 0, 0, 1) ); 
            }
            else
            {
                //outputData[inx] = 0; 
                SetPixelUI(output, i, j, glm::ivec4(0, 0, 0, 1) ); 
            }
        }
    }
    
    std::vector<glm::ivec2> countour; 

    static enum STATE {OUTSIDE, INSIDE} ;
    STATE state = OUTSIDE; 

    for(int j = 0; j < output->GetHeight(); j++)
    {
        for(int i = 0; i < output->GetWidth(); i++)
        {
            glm::ivec4 pixOut = GetPixelUI(output, i, j); 

            if(state == OUTSIDE)
            {
                if(pixOut.r != 0)
                {
                    state = INSIDE; 
                }

                if(pixOut.r == 1 || pixOut.r == 2 )
                {
                    //std::cout << "StartTRACE:" << i << " " << j << "\n";  
                    countour.clear(); 
                    TraceBorder(input, output, i, j, countour);
                }
            }
            else if(state == INSIDE)
            {
                if(pixOut.r == 0)
                {
                    state = OUTSIDE; 
                }
            } 
        }
    }

    //Cleanup
    for(int j = 0; j < output->GetHeight(); j++)
    {
        for(int i = 0; i < output->GetWidth(); i++)
        {
            glm::ivec4 pixOut = GetPixelUI(output, i, j); 
            if(pixOut.r == 4)
                SetPixel(output, i, j, glm::vec4(1, 1, 1, 1)); 
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
}