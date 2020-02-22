#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT StereoPairDepth
{
    private: 
        class Internal;
        Internal* internal;

	public:
		StereoPairDepth(); 
        ~StereoPairDepth(); 
		void Run(ImageGPU* inputL, ImageGPU* inputR, ImageGPU* output); 
        void Run(Image* inputL, Image* inputR, Image* output); 
};
	
}