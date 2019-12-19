#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT Normalize
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Normalize(); 
        ~Normalize(); 
        void Run(Image* input, Image* output); 
};
	
}