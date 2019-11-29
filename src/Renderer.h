#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{

class VISI_EXPORT Renderer
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Renderer(); 
        ~Renderer(); 
        void Clear();
        void AddCircle(); 
        void AddSquare(); 
        void AddLine(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}