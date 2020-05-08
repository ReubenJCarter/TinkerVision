#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{

class VISO_EXPORT Crop
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
}