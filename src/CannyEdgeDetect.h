#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT CannyEdgeDetect
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CannyEdgeDetect(); 
        ~CannyEdgeDetect(); 
        void SetBlurSigma(float sigm); 
        void SetHighEdgeThreshold(float t);
        void SetLowEdgeThreshold(float t);
		void Run(ImageGPU* input, Image* output); 
};
	
}