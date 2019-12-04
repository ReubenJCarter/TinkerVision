#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

#include <vector>
#include <glm/glm.hpp>

namespace Visi
{
	
class VISI_EXPORT FindContours
{
    private: 
        class Internal;
        Internal* internal;

	public:
        struct Contour
        {
            Contour* parent; 
            std::vector<glm::vec2> verticies; 
        }; 

        static void ContoursToFile(std::string fileName, std::vector<Contour>* contours); 
        static void ContoursFilter(std::vector<Contour>* input, std::vector<Contour>* output,
                                   int minVertCount=20); 
        static void ContoursSimplify(std::vector<Contour>* input, std::vector<Contour>* output,
                                     float keepDist); 
        static void ContoursMergeVerticies(std::vector<Contour>* input, std::vector<Contour>* output, float mergeDist);

		FindContours(); 
        ~FindContours(); 
        void Run(Image* input, Image* output, std::vector<Contour>* contours=NULL); 
};
	
}