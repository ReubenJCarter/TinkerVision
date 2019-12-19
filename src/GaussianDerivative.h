#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT GaussianDerivative
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Direction{HORIZONTAL, VERTICAL}; 

		GaussianDerivative(); 
        ~GaussianDerivative(); 
        void SetSigma(float s); 
        void SetDirection(Direction dir); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}