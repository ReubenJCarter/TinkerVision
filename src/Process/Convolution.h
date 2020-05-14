#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{

class TINKERVISION_EXPORT Convolution
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