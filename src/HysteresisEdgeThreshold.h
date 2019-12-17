#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT HysteresisEdgeThreshold
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