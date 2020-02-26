#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

#include <vector>

namespace Visi
{
	
class VISI_EXPORT ChannelMapper
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ChannelMapper(); 
        ~ChannelMapper(); 
        void Run(Image* input, Image* output); 
        void Run(ImageGPU* input, ImageGPU* output); 
        void SetChannelMap(int r=0, int g=1, int b=2, int a=3); //maps to output.r = input[r], ...etc
};
	
}