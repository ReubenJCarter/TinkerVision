#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

#include <vector>
#include <glm/glm.hpp>

namespace Visi
{
	
class VISI_EXPORT ApproxDistanceTransform
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ApproxDistanceTransform(); 
        ~ApproxDistanceTransform(); 
        void Run(Image* input, Image* output); 
};
	
}