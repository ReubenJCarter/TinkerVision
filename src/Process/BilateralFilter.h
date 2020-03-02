#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
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
}