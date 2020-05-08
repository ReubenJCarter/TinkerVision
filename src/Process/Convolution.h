#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{

class VISO_EXPORT Convolution
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Convolution(); 
        ~Convolution(); 
		void Run(ImageGPU* input, ImageGPU* conv, ImageGPU* output); 
        void Run(Image* input, Image* conv, Image* output); 
};
	
}
}