#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT Blend
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum BlendMode{BLEND_ADD, BLEND_SUBTRACT, BLEND_DIFFERENCE, BLEND_DIVIDE, BLEND_MULTIPLY, BLEND_OVERLAY};

		Blend(); 
        ~Blend(); 

        void SetMode(BlendMode bm);

		void Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output, ImageGPU* blendMask); 
        void Run(Image* inputSrc, Image* inputDst, Image* output, Image* blendMask); 

        void Run(ImageGPU* inputSrc, ImageGPU* inputDst, ImageGPU* output); 
        void Run(Image* inputSrc, Image* inputDst, Image* output); 
};
	
}
}