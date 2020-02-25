#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT Sobel
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Sobel(); 
        ~Sobel(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}