#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT Morph
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Morph(); 
        ~Morph(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}