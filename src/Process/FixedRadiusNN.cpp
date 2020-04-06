#include "FixedRadiusNN.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>

namespace Visi
{

namespace Process
{


class FixedRadiusNN::Internal
{
    public:
        struct HashInx
        {
            unsigned int inx;
            unsigned int hash;
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
		        if( itt->hash > hn[i] + 2) continue;


                while( itt->hash <= hn[i] + 2  &&  itt != hs.end())
                {
                    nns->push_back( itt->inx );
                    itt++;
                }
            }


        }   

        void SortByDistance(Vec2 point, std::vector<Vec2>* vecs, std::vector<int>* nns)
        {
            struct InxDist
            {
                int inx; 
                float dist;
            };
            std::vector<InxDist> distances2;
            distances2.reserve(nns->size());
            for(int i = 0; i < nns->size(); i++)
            {
                InxDist inxd; 
                Vec2 v = vecs->at(nns->at(i)); 
                float xdiff = (v.x - point.x);
                float ydiff = (v.y - point.y);
                inxd.dist = xdiff*xdiff + ydiff * ydiff;
                inxd.inx = nns->at(i); 
                distances2.push_back(inxd); 
            }
            std::sort(distances2.begin(), distances2.end(),[ ](const auto& lhs, const auto& rhs){ return lhs.dist < rhs.dist; });

            for(int i = 0; i < distances2.size(); i++)
            {
                nns->at(i) = distances2[i].inx; 
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


}

}