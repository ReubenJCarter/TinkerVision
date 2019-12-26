#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{

class VISI_EXPORT ClearColor
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ClearColor(); 
        ~ClearColor(); 
        void SetColor(float r, float g, float b, float a=1); 
		void Run(ImageGPU* output); 
        void Run(Image* output); 
};
	
}