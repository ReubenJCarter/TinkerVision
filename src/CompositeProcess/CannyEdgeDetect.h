#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace CompositeProcess
{
	
class VISO_EXPORT CannyEdgeDetect
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

        void RunGray(ImageGPU* input, ImageGPU* output); 
        void RunBlur(ImageGPU* input, ImageGPU* output); 
        void RunSobel(ImageGPU* input, ImageGPU* output); 
        void RunNMS(ImageGPU* input, ImageGPU* output); 
        void RunThresholding(Image* input, Image* output); 
};
	
}
}