#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT MorphRect
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode{ERODE, DILATE};

		MorphRect(); 
        ~MorphRect(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 

        void SetMode(Mode mode);
        void SetKernel(int w, int h);  
};
	
}
}