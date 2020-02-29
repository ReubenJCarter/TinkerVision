#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{

class VISI_EXPORT Crop
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Crop(); 
        ~Crop(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetOffset(int x, int y); 
};
	
}