#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include <vector>

namespace Visi
{
namespace CompositeProcess
{
	
class VISI_EXPORT Sift
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