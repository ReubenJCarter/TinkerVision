#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT Threshold
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