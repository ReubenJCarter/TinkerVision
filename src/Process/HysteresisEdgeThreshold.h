#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT HysteresisEdgeThreshold
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