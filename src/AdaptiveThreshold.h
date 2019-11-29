#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT AdaptiveThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		AdaptiveThreshold(); 
        ~AdaptiveThreshold(); 
        void SetThreshold(float t);
        void SetSize(int s); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}