#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT NonMaximumEdgeSuppression
{
    private: 
        class Internal;
        Internal* internal;

	public:
		NonMaximumEdgeSuppression(); 
        ~NonMaximumEdgeSuppression(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}