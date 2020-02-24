#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT HSVToRGB
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HSVToRGB(); 
        ~HSVToRGB(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}