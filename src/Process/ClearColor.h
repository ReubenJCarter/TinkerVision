#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
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
}