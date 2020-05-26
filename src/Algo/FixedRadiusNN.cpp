#include "FixedRadiusNN.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace TnkrVis
{

namespace Algo
{


class FixedRadiusNN::Internal
{
    public:
        struct HashInx
        {
            unsigned int inx;
            unsigned int hash;
        };

        struct DistanceInx
        {
            unsigned int inx;
            float dist; 
        }; 

        std::vector<HashInx> hs; 

        float cellSize; 

        void Build(std::vector<Vec2>* vecs, float radius)
        {
            hs.clear(); 
            hs.reserve(vecs->size()); 
            cellSize = radius; 
            for(unsigned int i = 0; i < vecs->size(); i++)
            {
                Vec2 v = vecs->at(i); 
                int X = v.x / cellSize; 
                int Y = v.y / cellSize; 
                X += 32768;
                Y += 32768; 
                unsigned int H = Y * 65536 + X; 

                HashInx h;
                h.inx = i;
                h.hash = H; 
                hs.push_back(h); 
            }
            std::sort(hs.begin(), hs.end(),[ ](const auto& lhs, const auto& rhs){ return lhs.hash < rhs.hash; });
        }

        void Test(Vec2 point, std::vector<int>* nns)
        {
            int X = point.x / cellSize; 
            int Y = point.y / cellSize; 
            X += 32768;
            Y += 32768; 

            unsigned int hn[3]; 
            hn[0] = (Y + 1) * 65536 + X-1; 
            hn[1] = (Y) * 65536 +     X-1; 
            hn[2] = (Y - 1) * 65536 + X-1; 
            for(int i = 0; i < 3; i++)
            {
                //first hash not less that the h[i]
                auto itt = std::lower_bound(hs.begin(), hs.end(), hn[i], [](const HashInx& lhs, const unsigned int rhs){ return lhs.hash < rhs; });
                
                //nothing found
                if(itt == hs.end()) continue;

                //out of range
		        if( (*itt).hash > hn[i] + 2) continue;


                while( (*itt).hash <= hn[i] + 2)
                {
                    nns->push_back( (*itt).inx );
                    itt++;
                    if(itt == hs.end())
                        break; 
                }
            }


        }   

        void SortByDistance(Vec2 point, std::vector<Vec2>* vecs, std::vector<int>* nns)
        {
            std::vector<DistanceInx> distances2;
            distances2.reserve(nns->size());
            for(int i = 0; i < nns->size(); i++)
            {
                DistanceInx inxd; 
                Vec2 v = vecs->at(nns->at(i)); 
                float xdiff = (v.x - point.x);
                float ydiff = (v.y - point.y);
                inxd.dist = xdiff * xdiff + ydiff * ydiff;
                inxd.inx = nns->at(i); 
                distances2.push_back(inxd); 
            }
            std::sort(distances2.begin(), distances2.end(),[ ](const auto& lhs, const auto& rhs){ return lhs.dist < rhs.dist; });

            for(int i = 0; i < distances2.size(); i++)
            {
                nns->at(i) = distances2[i].inx; 
            }
        }

        void RadialOutlierRemoval(std::vector<Vec2>* vecs, float minRadius, float maxRadius, int minNsWithinBounds)
        {
            Build(vecs, maxRadius); 
            std::vector<TnkrVis::Vec2> newVerticies;
            newVerticies.reserve(vecs->size()); 
            float minRadius2 = minRadius*minRadius;
            float maxRadius2 = maxRadius*maxRadius;
            for(int i = 0; i < vecs->size(); i++)
            {
                TnkrVis::Vec2 vc = vecs->at(i);
                std::vector<int> nns;
                Test(vc, &nns); 
                int validNCount = 0;
                for(int j = 0; j < nns.size(); j++)
                {
                    if(nns[j] == i)
                        continue; 

                    TnkrVis::Vec2 n = vecs->at( nns[j] );
                    float dist = (n - vc).Length2(); 
                    
                    if(dist > minRadius2 && dist < maxRadius2)
                    { 
                        validNCount++; 
                    }
                }
                if(validNCount >= minNsWithinBounds)	
                {
                    newVerticies.push_back(vc); 
                }
            }

            vecs->clear(); 
            for(int i = 0; i < newVerticies.size(); i++)
            {
                vecs->push_back(newVerticies[i]); 
            }
        }
};


FixedRadiusNN::FixedRadiusNN()
{
    internal = new Internal(); 
}

FixedRadiusNN::~FixedRadiusNN()
{
    delete internal; 
}

void FixedRadiusNN::Build(std::vector<Vec2>* vecs, float radius)
{
    internal->Build(vecs, radius); 
}

void FixedRadiusNN::Test(Vec2 point, std::vector<int>* nns)
{
    internal->Test(point, nns); 
}

void FixedRadiusNN::SortByDistance(Vec2 point, std::vector<Vec2>* vecs, std::vector<int>* nns)
{
    internal->SortByDistance(point, vecs, nns); 
}

void FixedRadiusNN::RadialOutlierRemoval(std::vector<Vec2>* vecs, float minRadius, float maxRadius, int minNsWithinBounds)
{
    internal->RadialOutlierRemoval(vecs, minRadius, maxRadius, minNsWithinBounds); 
}


}

}