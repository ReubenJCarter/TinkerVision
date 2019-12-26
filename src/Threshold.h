#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

#include "VectorMath.h"

namespace Visi
{
	
class VISI_EXPORT Threshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Threshold(); 
        ~Threshold(); 
        void SetThreshold(float t);
        void SetThreshold(Color t);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}