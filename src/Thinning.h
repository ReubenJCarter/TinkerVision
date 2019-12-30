#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT Thinning
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Thinning(); 
        ~Thinning(); 
        void Run(Image* input, Image* output); 
};
	
}