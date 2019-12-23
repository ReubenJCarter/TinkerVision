#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

#include <glm/glm.hpp>

namespace Visi
{
	
class VISI_EXPORT HighLowThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HighLowThreshold(); 
        ~HighLowThreshold(); 
        void SetLowThreshold(float t);
        void SetHighThreshold(float t);
        void SetLowThreshold(glm::vec3 t);
        void SetHighThreshold(glm::vec3 t);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}