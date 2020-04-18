#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT MorphRect
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