#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
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
}