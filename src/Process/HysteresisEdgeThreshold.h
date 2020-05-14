#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT HysteresisEdgeThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HysteresisEdgeThreshold(); 
        ~HysteresisEdgeThreshold(); 
        void Run(Image* input, Image* output); 
        void SetHighThreshold(float highThr);
        void SetLowThreshold(float lowThr); 
};
	
}
}