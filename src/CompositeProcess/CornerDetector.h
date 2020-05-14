#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace CompositeProcess
{
	
class TINKERVISION_EXPORT CornerDetector
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CornerDetector(); 
        ~CornerDetector(); 
        
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}