#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT BilateralFilter
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BilateralFilter(); 
        ~BilateralFilter(); 
        void SetSigma(float s);
        void SetSize(int s); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}