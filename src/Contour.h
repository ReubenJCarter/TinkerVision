#pragma once

#include "Visi_export.h"

#include <vector>
#include "VectorMath.h"

namespace Visi
{
	
class VISI_EXPORT Contour
{
        public:
                Contour* parent; 
                std::vector<Vec2> verticies; 
                
                static void ContoursToFile(std::string fileName, std::vector<Contour>* contours); 
                static void ContoursVertCountFilter(std::vector<Contour>* input, std::vector<Contour>* output,
                                        int minVertCount=20, int maxVertCount=-1); 
                static void ContoursSimplify(std::vector<Contour>* input, std::vector<Contour>* output,
                                        float keepDist); 
                static void ContoursMergeVerticies(std::vector<Contour>* input, std::vector<Contour>* output, float mergeDist);
};
	
}