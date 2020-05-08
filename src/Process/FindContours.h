#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include <vector>

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT FindContours
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
}