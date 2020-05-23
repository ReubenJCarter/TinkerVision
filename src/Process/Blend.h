#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT Blend
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum BlendMode{BLEND_ADD, BLEND_SUBTRACT, BLEND_DIFFERENCE, BLEND_DIVIDE, BLEND_MULTIPLY, BLEND_OVERLAY};

		Blend(); 
        ~Blend(); 

        void SetMode(BlendMode bm);

		void Run(ImageGPU* inputTop, ImageGPU* inputBottom, ImageGPU* output, ImageGPU* blendMask); 
        void Run(Image* inputTop, Image* inputBottom, Image* output, Image* blendMask); 

        void Run(ImageGPU* inputTop, ImageGPU* inputBottom, ImageGPU* output); 
        void Run(Image* inputTop, Image* inputBottom, Image* output); 
};
	
}
}