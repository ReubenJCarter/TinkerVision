#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

#include "VectorMath.h"

namespace Visi
{
	
class VISI_EXPORT HighLowThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HighLowThreshold(); 
        ~HighLowThreshold(); 
        void SetLowThreshold(float t);
        void SetHighThreshold(float t);
        void SetLowThreshold(Color t);
        void SetHighThreshold(Color t);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}