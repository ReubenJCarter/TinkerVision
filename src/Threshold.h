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
	
class VISI_EXPORT Threshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Threshold(); 
        ~Threshold(); 
        void SetLevel(float t); 
		void Run(ImageGPU* input, ImageGPU* output); 
};
	
}