#include "Contour.h"

#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <functional>
#include <algorithm>
#include <stack>


#include <glm/glm.hpp>

namespace Visi
{

Contour::Contour()
{
    closed = true; 
}

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

        glm::vec2 startPoint = glm::vec2(c->verticies[inxStart].x, c->verticies[inxStart].y);
        glm::vec2 endPoint = glm::vec2(c->verticies[inxStart + count -1].x, c->verticies[inxStart + count -1].y); 
        glm::vec2 B = glm::normalize(endPoint - startPoint); 

        //loop over points and find the furthest perpendicular dist from 
        //the start and end of the segment
        float maxDist = 0; 
        int maxDistInx = -1; 
        for(int i = 1; i < count-1; i++)
        {
            int inx = inxStart + i;
            glm::vec2 point = glm::vec2(c->verticies[inx].x, c->verticies[inx].y);

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

        glm::vec2 startPoint = glm::vec2(c.verticies[0].x, c.verticies[0].y);
        glm::vec2 endPoint = glm::vec2(c.verticies[c.verticies.size() - 1].x, c.verticies[c.verticies.size() - 1].y);

        cOut.verticies.clear();
    
        //start recursion
        recure(&c, &cOut, 0, c.verticies.size());

        //if there is 1 or 0 points in cout, not need to simplify 
        if(cOut.verticies.size() < 2)
        {
            cOut.verticies.push_back(Vec2(endPoint.x, endPoint.y)); 
            cOut.verticies.push_back(Vec2(startPoint.x, startPoint.y)); 
            continue; 
        }

        // we want [last-1 last start start+1] Then "recure" on the 4 elements
        //at this point we have [start+1 ..... last-1] in cout
        //This only works if you assumen tthe contour is a closed polygon.         
        
        

        Contour temp1;
        Contour temp2;
        temp1.verticies.push_back(cOut.verticies[cOut.verticies.size()-1]); 
        temp1.verticies.push_back(Vec2(endPoint.x, endPoint.y)); 
        temp1.verticies.push_back(Vec2(startPoint.x, startPoint.y)); 
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
        glm::vec2 vPrev = glm::vec2(c->verticies[c->verticies.size()-1].x, c->verticies[c->verticies.size()-1].y);

        //cOut->verticies.push_back(vPrev); 

        glm::vec2 av = vPrev; 
        int avCount = 1; 
        for(int i = 0; i < c->verticies.size(); i++)
        {
            glm::vec2 v = glm::vec2(c->verticies[i].x, c->verticies[i].y);
            glm::vec2 diff = v - vPrev; 
            if(!(glm::dot(diff, diff) < dist2))
            {
                //add to new countout vert list
                av/=avCount;
                cOut->verticies.push_back(Vec2(av.x, av.y)); 
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

//https://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
void Contour::FindConvexHull(Contour* contour, std::vector<int>* convexHull)
{//TODO DOESNT YET USE DISTANCE FOR SORT WHEN ORIENTATIONS ARE THE SAME 
    convexHull->clear();

    //find p0
    float P0Y = 100000000;
    int P0Inx = -1; 
    float P0X = 0;
    for(int i = 0; i < contour->verticies.size(); i++)
    {
        if(contour->verticies[i].y < P0Y)
        {
            P0Y = contour->verticies[i].y;
            P0X = contour->verticies[i].x;
            P0Inx = i; 
        }
        else if(contour->verticies[i].y == P0Y) //if same y choose least x
        {
            if(contour->verticies[i].x < P0X)
            {
                P0Y = contour->verticies[i].y;
                P0X = contour->verticies[i].x;
                P0Inx = i; 
            }
        }
    }

    //Find polar type coord (actually find gradient in each quadrent)
    struct PolarInx
    {
        int inx;
        float angle;
        Vec2 v;
    }; 
    std::vector<PolarInx> pInxs;
    for(int i = 0; i < contour->verticies.size(); i++)
    {
        if(i == P0Inx)
            continue; //skip if inx is P0

        float run = contour->verticies[i].x - P0X; 
        float rise = contour->verticies[i].y - P0Y; 

        float gradThing; 

        if(run > 0 && abs(run) >= abs(rise)) //Quadrent 1
        {
            gradThing = rise / run; //0 to 1
        }
        else if(run > 0 && abs(rise) > abs(run))
        {
            gradThing = 1 + (1 - run / rise); // 1 + 0 to 1
        }
        else if(run <= 0 && abs(rise) >= abs(run))
        {
            gradThing = 2 + (abs(run) / rise); // 2 + 0 to 1
        }
        else
        {
            gradThing = 3 + (1 - abs(rise) / abs(run));
        }
        PolarInx pinx; 
        pinx.angle = gradThing; 
        pinx.inx = i;
        pinx.v = contour->verticies[i];
        pInxs.push_back(pinx); 
    }

    //Sort on polar
    std::sort(pInxs.begin(), pInxs.end(), [](PolarInx a, PolarInx b) {return a.angle < b.angle; });

   

    //
    // To find orientation of ordered triplet (p, q, r). 
    // The function returns following values 
    // 0 --> p, q and r are colinear 
    // 1 --> Clockwise 
    // 2 --> Counterclockwise 
    auto Orientation = [](Vec2 p, Vec2 q, Vec2 r) -> int 
    { 
        int val = (q.y - p.y) * (r.x - q.x) - 
                (q.x - p.x) * (r.y - q.y); 
    
        if (val == 0) return 0;  // colinear 
        return (val > 0)? 1: 2; // clock or counterclock wise 
    } ;


     //Remove points that are the same angel with P0 
    int m = 1; // Initialize size of modified array 
    int n = pInxs.size();
    for (int i=1; i<n; i++) 
    { 
        // Keep removing i while angle of i and i+1 is same 
        // with respect to p0 
        while (i < n-1 && Orientation(Vec2(P0X, P0Y), pInxs[i].v,  pInxs[i+1].v) == 0) 
            i++; 
    
    
        pInxs[m] = pInxs[i]; 
        m++;  // Update size of modified array 
    } 
    int count = m;
    

    // If modified array of points has less than 3 points, 
    // convex hull is not possible 
    if (count < 3) return; 
    
    // Create an empty stack and push first three points 
    // to it. 
    std::stack<PolarInx> S; 
    S.push(pInxs[0]); 
    S.push(pInxs[1]); 
    S.push(pInxs[2]); 
    

    auto nextToTop = [](std::stack<PolarInx> &S) -> PolarInx
    { 
        PolarInx p = S.top(); 
        S.pop(); 
        PolarInx res = S.top(); 
        S.push(p); 
        return res; 
    } ;
    
    // Process remaining n-3 points 
    for (int i = 3; i < count; i++) 
    { 
        // Keep removing top while the angle formed by 
        // points next-to-top, top, and points[i] makes 
        // a non-left turn 
        while (Orientation(nextToTop(S).v, S.top().v, pInxs[i].v) != 2) 
            S.pop(); 
        S.push(pInxs[i]); 
    } 
    
    // Now stack has the output points, print contents of stack 
    convexHull->push_back(P0Inx); 
    while (!S.empty()) 
    { 
        PolarInx p = S.top(); 
        convexHull->push_back(p.inx); 
        S.pop(); 
    } 
}

BoundingBox Contour::FindBoundingBox(Contour* contour)
{
    //Find the convex hull ???
    std::vector<int> convx;
    FindConvexHull(contour, &convx);

    struct AABB
    {
        glm::vec2 min;
        glm::vec2 max;
    }; 

	auto FindAABB = [](Contour* contour, glm::mat2 rotation) -> AABB
	{	
		glm::vec2 AABBmin = glm::vec2(100000000, 100000000);
		glm::vec2 AABBmax = glm::vec2(-100000000, -100000000);
		
		for(int i = 0; i < contour->verticies.size(); i++)
		{
			glm::vec2 P = rotation * glm::vec2(contour->verticies[i].x, contour->verticies[i].y); 
			AABBmin.x = P.x < AABBmin.x ? P.x : AABBmin.x; 
			AABBmin.y = P.y < AABBmin.y ? P.y : AABBmin.y; 
			AABBmax.x = P.x > AABBmax.x ? P.x : AABBmax.x; 
			AABBmax.y = P.y > AABBmax.y ? P.y : AABBmax.y; 
		}
        AABB aabb;
        aabb.min = AABBmin;
        aabb.max = AABBmax; 
        return aabb; 
	};
	
    float minArea = 1000000000; 
    BoundingBox minAreaBB; 

	for(int i = 0; i < convx.size(); i++)
    {
        int aInx = convx[i]; 
        int bInx = convx[(i+1) % convx.size()]; 
        Vec2 a = contour->verticies[aInx];
        Vec2 b = contour->verticies[bInx]; 

        float run = b.x - a.x; 
        float rise = b.y - a.y; 
        float angle = atan2(rise, run); 

        const static float RADTODEG = 57.2958;
        float sinAng = sin(angle );
        float cosAng = cos(angle );

        glm::mat2 rotMat; 
        rotMat[0] = glm::vec2(cosAng, sinAng); 
        rotMat[1] = glm::vec2(-sinAng, cosAng); 
        AABB aabb = FindAABB(contour, rotMat ); 
        glm::vec2 pos = glm::vec2((aabb.max.x + aabb.min.x) / 2, (aabb.max.y + aabb.min.y) / 2); 
        glm::vec2 exte = glm::vec2((aabb.max.x - aabb.min.x) / 2, (aabb.max.y - aabb.min.y) / 2);
        float area = (aabb.max.x - aabb.min.x) * (aabb.max.y - aabb.min.y);
        if(area < minArea)
        {
            minArea = area; 
            minAreaBB.position = Vec2(pos.x, pos.y); 
            minAreaBB.rotation = angle * RADTODEG;
            minAreaBB.extends = Vec2(exte.x, exte.y);
        }
    }
	/*
    std::cout << "Min Area BB:position:" << minAreaBB.position.x << " " << minAreaBB.position.y
              << " extend:" << minAreaBB.extends.x << " " << minAreaBB.extends.y  
              << " rotation:" << minAreaBB.rotation
              << "\n";*/
	return minAreaBB;
}

}