#include "FindContours.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <fstream>

namespace Visi
{

class FindContours::Internal
{
    private:

        void TraceBorder(Image* im, int startI, int startJ, int polyID, Contour* contour); 

    public:
        Internal(); 
        void Run(Image* input, Image* output, std::vector<Contour>* contours);
};


FindContours::Internal::Internal()
{
}

void FindContours::Internal::TraceBorder(Image* im, int startI, int startJ, int polyID, Contour* contour)
{
    uint16_t* outputData = (uint16_t*)im->GetData(); 
    static enum DIRECTION{N, NE, E, SE, S, SW, W, NW};
    
    int ladybirdI = startI;
    int ladybirdJ = startJ; 
    int startCounter = 0;
    int rotateCounter = 0; 
    DIRECTION direction = N;
    int p1, p2, p3;

    while(startCounter < 2 && rotateCounter <= 3)
    {
        if(direction == N)
        {
            p1 = GetPixelUI(im, ladybirdI - 1, ladybirdJ + 1).r;  
            p2 = GetPixelUI(im, ladybirdI,     ladybirdJ + 1).r;  
            p3 = GetPixelUI(im, ladybirdI + 1, ladybirdJ + 1).r;
            if(p1 > 0)
            {
                ladybirdI -= 1;
                ladybirdJ += 1; 
                direction = W;    
                rotateCounter = 0;                            
            }  
            else if(p2 > 0)
            {
                ladybirdJ += 1; 
                direction = N;  
                rotateCounter = 0; 
            }
            else if(p3 > 0)
            {
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
            p1 = GetPixelUI(im, ladybirdI + 1, ladybirdJ - 1).r;  
            p2 = GetPixelUI(im, ladybirdI,     ladybirdJ - 1).r;  
            p3 = GetPixelUI(im, ladybirdI - 1, ladybirdJ - 1).r;  

            if(p1 > 0)
            {
                ladybirdI += 1;
                ladybirdJ -= 1; 
                direction = E;   
                rotateCounter = 0;                              
            }  
            else if(p2 > 0)
            {
                ladybirdJ -= 1; 
                direction = S;  
                rotateCounter = 0; 
            }
            else if(p3 > 0)
            {
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
            p1 = GetPixelUI(im, ladybirdI + 1, ladybirdJ + 1).r;  
            p2 = GetPixelUI(im, ladybirdI + 1, ladybirdJ).r;  
            p3 = GetPixelUI(im, ladybirdI + 1, ladybirdJ - 1).r;  

            if(p1 > 0)
            {
                ladybirdI += 1;
                ladybirdJ += 1; 
                direction = N;    
                rotateCounter = 0;                            
            }  
            else if(p2 > 0)
            {
                ladybirdI += 1; 
                direction = E;  
                rotateCounter = 0; 
            }
            else if(p3 > 0)
            {
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
            p1 = GetPixelUI(im, ladybirdI - 1, ladybirdJ - 1).r;  
            p2 = GetPixelUI(im, ladybirdI - 1, ladybirdJ).r;  
            p3 = GetPixelUI(im, ladybirdI - 1, ladybirdJ + 1).r;  

            if(p1 > 0)
            {
                ladybirdI -= 1;
                ladybirdJ -= 1; 
                direction = S;      
                rotateCounter = 0;                          
            }  
            else if(p2 > 0)
            {
                ladybirdI -= 1; 
                direction = W;  
                rotateCounter = 0;
            }
            else if(p3 > 0)
            {
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
        int inx = im->GetWidth() * ladybirdJ + ladybirdI; 
        outputData[inx] = polyID;
        if(contour != NULL)
        {
            contour->verticies.push_back(glm::vec2(ladybirdI, ladybirdJ)); 
        }
    }
}


void FindContours::Internal::Run(Image* input, Image* output, std::vector<Contour>* contours)
{
    if(output->GetWidth() != input->GetWidth() || output->GetHeight() != input->GetHeight() || output->GetType() != ImageType::GRAYSCALE16) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), ImageType::GRAYSCALE16); 
    }
    uint16_t* outputData = (uint16_t*)output->GetData(); 

    //copy threshold to output image
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            int inx = j * input->GetWidth() + i; 
            glm::vec4 pix = GetPixel(input, i, j); 
            if(pix.r > 0.5f)
            {
                outputData[inx] = 1; 
            }
            else
            {
                outputData[inx] = 0; 
            }
        }
    }

    static enum STATE {OUTSIDE, INSIDE} ;
    
    
    STATE state = OUTSIDE; 
    int polyID = 30000; 
    
    for(int j = 0; j < input->GetHeight(); j++)
    {
        for(int i = 0; i < input->GetWidth(); i++)
        {
            int inx = j * input->GetWidth() + i; 
            int pix = outputData[inx]; 

            if(state == OUTSIDE)
            {
                if(pix != 0)
                {
                    state = INSIDE; 
                }

                if(pix == 1)
                {
                    polyID++; 
                    if(contours != NULL)
                    {
                        contours->push_back(Contour()); 
                        Contour* c = &contours->at(contours->size()-1);
                        TraceBorder(output, i, j, polyID, c);
                    }
                    else
                    {
                        TraceBorder(output, i, j, polyID, NULL);
                    }
                }
            }
            else //state is inside
            {
                if(pix == 0)
                {
                    state = OUTSIDE; 
                }
            }
        } 
    } 
}




FindContours::FindContours()
{
    internal = new Internal(); 
}

FindContours::~FindContours()
{ 
    delete internal; 
}

void FindContours::Run(Image* input, Image* output, std::vector<Contour>* contours)
{
    internal->Run(input, output, contours); 
}

void FindContours::ContoursToFile(std::string fileName, std::vector<Contour>* contours)
{
    std::ofstream ofstream; 
    ofstream.open(fileName); 
    if(!ofstream.is_open())
    {
        std::cerr << "Visi:FindContours::ContoursToFile:Could not open file for writting:" << fileName << "\n";
        return; 
    }

    for(int i = 0; i < contours->size(); i++)
    {
        Contour& c = contours->at(i);
        ofstream << "Contour:" << i << "\n";
        for(int j = 0; j < c.verticies.size(); j++)
        {
            ofstream << c.verticies[j].x << " " << c.verticies[j].y << "\n";
        }
    }
    ofstream.close(); 
}

void FindContours::ContoursFilter(std::vector<Contour>* input, std::vector<Contour>* output, int minVertCount)
{

    for(int i = 0; i < input->size(); i++)
    {
        Contour& c = input->at(i);
        if(c.verticies.size() >= minVertCount)
        {
            output->push_back(Contour()); 
            Contour& oc = output->at(output->size()-1);

            oc.verticies.resize(c.verticies.size()); 
            for(int j = 0; j < c.verticies.size(); j++)
            {
                oc.verticies[j] = c.verticies[j]; 
            }
        }
    }
}

}