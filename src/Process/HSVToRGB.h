#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

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