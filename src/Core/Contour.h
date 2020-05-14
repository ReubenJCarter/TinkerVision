#pragma once

#include "TinkerVision_export.h"

#include <vector>
#include "VectorMath.h"
#include "BoundingBox.h"

namespace TnkrVis
{
	
class TINKERVISION_EXPORT Contour
{
        public:
                Contour* parent; 
                std::vector<Vec2> verticies; 
                bool closed; 

                Contour();

                static void ContoursToFile(std::string fileName, std::vector<Contour>* contours); 
                static void ContoursVertCountFilter(std::vector<Contour>* input, std::vector<Contour>* output,
                                        int minVertCount=20, int maxVertCount=-1); 
                static void ContoursSimplify(std::vector<Contour>* input, std::vector<Contour>* output,
                                        float keepDist); 
                static void ContoursMergeVerticies(std::vector<Contour>* input, std::vector<Contour>* output, float mergeDist);
                static void FindConvexHull(Contour* contour, std::vector<int>* convexHull); 
                static BoundingBox FindBoundingBox(Contour* contour); 
                static Vec2 FindMeanVertex(Contour* contour); 
};
	
}