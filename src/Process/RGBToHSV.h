#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT RGBToHSV
{
    private: 
        class Internal;
        Internal* internal;

	public:
		RGBToHSV(); 
        ~RGBToHSV(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}