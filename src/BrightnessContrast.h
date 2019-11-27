#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{

/*
 *Sobel
 *
 */
	
class VISI_EXPORT BrightnessContrast
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
};
	
}