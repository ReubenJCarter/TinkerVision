#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

namespace Visi
{
namespace Process
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
}