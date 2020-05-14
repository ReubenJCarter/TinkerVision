#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT ApproxDistanceTransform
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ApproxDistanceTransform(); 
        ~ApproxDistanceTransform(); 
        void Run(Image* input, Image* output); 
};
	
}
}