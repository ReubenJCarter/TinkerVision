#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{

class VISI_EXPORT Convolution
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