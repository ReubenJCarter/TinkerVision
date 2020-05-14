#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{

class TINKERVISION_EXPORT BrightnessContrast
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BrightnessContrast(); 
        ~BrightnessContrast(); 
        void SetBrightness(float b); 
        void SetContrast(float c); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}