#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{

class TINKERVISION_EXPORT Upsample
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode {NEAREST, BILINEAR};

		Upsample(); 
        ~Upsample(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetMode(Mode m); 
};
	
}
}