#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include <vector>

namespace TnkrVis
{
namespace CompositeProcess
{
	
class TINKERVISION_EXPORT Sift
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Sift(); 
        ~Sift(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        std::vector<ImageGPU>* GetPyramid(); 
};
	
}
}