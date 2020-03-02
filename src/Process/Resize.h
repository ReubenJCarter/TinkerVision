#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "Upsample.h"
#include "Downsample.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT Resize
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Resize(); 
        ~Resize(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetUpsampleMode(Upsample::Mode m);
        void SetDownsampleMode(Downsample::Mode m); 
};
	
}
}