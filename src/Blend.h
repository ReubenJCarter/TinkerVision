#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT Blend
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum BlendMode{ADDITION, SUBTRACT, DIFFERENCE, DIVIDE, MULTIPLY, ALPHABLEND};

		Blend(); 
        ~Blend(); 

        void SetMode(BlendMode bm);

		void Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output); 
        void Run(Image* inputSrc, Image* inputDst, Image* output); 
};
	
}