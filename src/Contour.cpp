#include "Contour.h"

#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <functional>

namespace Visi
{

void Contour::ContoursToFile(std::string fileName, std::vector<Contour>* contours)
{
    std::ofstream ofstream; 
    ofstream.open(fileName); 
    if(!ofstream.is_open())
    {
        std::cerr << "Visi:Contour::ContoursToFile:Could not open file for writting:" << fileName << "\n";
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

void Contour::ContoursVertCountFilter(std::vector<Contour>* input, std::vector<Contour>* output, int minVertCount, int maxVertCount)
{
    output->clear(); 
    for(int i = 0; i < input->size(); i++)
    {
        Contour& c = input->at(i);
        if(c.verticies.size() >= minVertCount || minVertCount < 0)
        {
            if(c.verticies.size() <= maxVertCount || maxVertCount < 0)
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

void Contour::ContoursSimplify(std::vector<Contour>* input, std::vector<Contour>* output,
                                    float keepDist)
{


    std::function<void(Contour*, Contour*, int, int)> recure = [&recure, keepDist ](Contour* c, Contour* cOut, int inxStart, int count)
    {
        if(count <= 2)
        {
            return; 
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
                maxDistInx = inx;
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
            recure(c, cOut, maxDistInx, count - (maxDistInx - inxStart));  
        }
    };

    output->resize(input->size()); 

    for(int i = 0; i < input->size(); i++)
    {
        Contour& c = input->at(i);
        Contour& cOut = output->at(i);

        //continue if contour has 2 or fewer verts (no point simplifying)
        if(c.verticies.size() <= 2)
        {
            continue; 
        }

        glm::vec2 startPoint = c.verticies[0];
        glm::vec2 endPoint = c.verticies[c.verticies.size() - 1];

        cOut.verticies.clear();
    
        //start recursion
        recure(&c, &cOut, 0, c.verticies.size());

        //if there is 1 or 0 points in cout, not need to simplify 
        if(cOut.verticies.size() < 2)
        {
            cOut.verticies.push_back(endPoint); 
            cOut.verticies.push_back(startPoint); 
            continue; 
        }

        // we want [last-1 last start start+1] Then "recure" on the 4 elements
        //at this point we have [start+1 ..... last-1] in cout
        //This only works if you assumen tthe contour is a closed polygon.         
        
        

        Contour temp1;
        Contour temp2;
        temp1.verticies.push_back(cOut.verticies[cOut.verticies.size()-1]); 
        temp1.verticies.push_back(endPoint); 
        temp1.verticies.push_back(startPoint); 
        temp1.verticies.push_back(cOut.verticies[0]); 

        recure(&temp1, &temp2, 0, temp1.verticies.size());

        //the start and end points may be present in the mini contour and need to be added to the end of the list temp2=[endPOint, startPoint]
        for(int j = 0; j < temp2.verticies.size(); j++)
        {
            cOut.verticies.push_back(temp2.verticies[j]); 
        }
        
    }
    

}

void Contour::ContoursMergeVerticies(std::vector<Contour>* input, std::vector<Contour>* output, float mergeDist)
{
    auto MergeVerticies = [](Contour* c, Contour* cOut, float dist2)
    {
        if(c->verticies.size() <= 1)
            return; 
        glm::vec2 vPrev = c->verticies[0];

        //cOut->verticies.push_back(vPrev); 

        glm::vec2 av = vPrev; 
        int avCount = 1; 
        for(int i = 1; i < c->verticies.size(); i++)
        {
            glm::vec2 v = c->verticies[i];
            glm::vec2 diff = v - vPrev; 
            if(!(glm::dot(diff, diff) < dist2))
            {
                //add to new countout vert list
                av/=avCount;
                cOut->verticies.push_back(av); 
                vPrev = v;
                av = vPrev; 
                avCount = 1; 
            }
            else
            {
                av += v; 
                avCount++; 
            }
        }

        
    };


    output->resize(input->size()); 
    float dist2 = mergeDist * mergeDist; 
    for(int i = 0; i < input->size(); i++)
    {
        Contour& c = input->at(i);
        Contour& cOut = output->at(i);
        cOut.verticies.clear();

        MergeVerticies(&c, &cOut, dist2);
    }
}

}