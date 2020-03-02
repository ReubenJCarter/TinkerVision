#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
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
}