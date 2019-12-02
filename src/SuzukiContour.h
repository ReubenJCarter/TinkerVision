#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT SuzukiContour
{
    private: 
        class Internal;
        Internal* internal;

	public:
		SuzukiContour(); 
        ~SuzukiContour(); 
        void Run(Image* input, Image* output); 
};
	
}