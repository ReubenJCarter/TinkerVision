#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"
#include "Contour.h"

#include <vector>

namespace Visi
{
	
class VISI_EXPORT FindContours
{
    private: 
        class Internal;
        Internal* internal;

	public:
		FindContours(); 
        ~FindContours(); 
        void Run(Image* input, Image* output, std::vector<Contour>* contours=NULL); 
};
	
}