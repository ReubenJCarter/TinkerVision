#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT Sobel
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode{FULL, GRAD_ONLY, MAG_ONLY};

		Sobel(); 
        ~Sobel(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetMode(Mode m); 
};
	
}
}