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
	
class VISI_EXPORT Sobel
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Sobel(); 
        ~Sobel(); 
		void Run(ImageGPU* input, ImageGPU* output); 
};
	
}