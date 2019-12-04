#include "FindContours.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <functional>

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
        if(outputData[inx] != polyID) //new ground
        {
            outputData[inx] = polyID;
            if(contour != NULL)
            {
                contour->verticies.push_back(glm::vec2(ladybirdI, ladybirdJ)); 
            }
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
    
    contours->clear();  

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
                    polyID += 1; 
                    if(contours != NULL)
                    {
                        contours->push_back(Contour()); 
                        Contour* c = &contours->at(contours->size()-1);
                        TraceBorder(output, i, j, polyID, c);
                        //SetPixel(output, i, j, glm::vec4(1, 0, 0, 0)); //Mark start pixel
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
    output->clear(); 
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

void FindContours::SimplifyContours(std::vector<Contour>* input, std::vector<Contour>* output,
                                    float keepDist)
{


    std::function<void(Contour*, Contour*, int, int)> recure = [&recure, input, keepDist ](Contour* c, Contour* cOut, int inxStart, int count)
    {
        if(count < 2)
        {

        }

        glm::vec2 startPoint = c->verticies[inxStart];
        glm::vec2 endPoint = c->verticies[inxStart + count -1]; 
        glm::vec2 B = glm::normalize(endPoint - startPoint); 

        //loop over points and find the furthest perpendicular dist from 
        //the start and end of the segment
        float maxDist = 0; 
        int maxDistInx = -1; 
        for(int i = 1; i < count-1; i++)
        {
            int inx = inxStart + i;
            glm::vec2 point = c->verticies[inx];

            //compute per dist squared 
            glm::vec2 A = point - startPoint; 
            float Alen2 = glm::dot(A, A);  //square length
            float proj = glm::dot(A, B); 
            float dist2 = Alen2 - proj * proj;

            //test against max
            if(dist2 > maxDist)
            {
                maxDist = dist2;
                maxDistInx = i;
            }
        }

        //sqrt to get actual dist
        maxDist = sqrt(maxDist); 

        // if the perp dist is greater than the keep dist, recure on each segnment
        if(maxDist > keepDist && maxDistInx != -1)
        {
            //first part of polyline
            recure(c, cOut, inxStart, maxDistInx - inxStart + 1); 

            //Add max point to new polyline
            cOut->verticies.push_back(c->verticies[maxDistInx]); 

            //second part of polyline
            recure(c, cOut, maxDistInx, count - maxDistInx); 
        }
    };

    auto MergeVerticies = [](Contour* c, float dist2)
    {
        if(c->verticies.size() <= 1)
            return; 
        std::vector<glm::vec2> newVerts; 
        glm::vec2 vPrev = c->verticies[0];
        for(int i = 1; i < c->verticies.size(); i++)
        {
            glm::vec2 v = c->verticies[i];
            glm::vec2 diff = v - vPrev; 
            if(!(glm::dot(diff, diff) < dist2))
            {
                //add to new countout vert list
                newVerts.push_back(v); 
                vPrev = v;
            }
        }

        c->verticies = newVerts; 
    };

    output->resize(input->size()); 

    for(int i = 0; i < input->size(); i++)
    {
        Contour& c = input->at(i);
        Contour& cOut = output->at(i);
        cOut.verticies.clear();

        //add the start vertex
        glm::vec2 startPoint = c.verticies[0];
        cOut.verticies.push_back(startPoint);

        //start recursion
        recure(&c, &cOut, 0, c.verticies.size());

        glm::vec2 endPoint = c.verticies[c.verticies.size() - 1]; 
        cOut.verticies.push_back(endPoint);

        //remove duplicates
        MergeVerticies(&cOut, 3);
    }
    

}

}