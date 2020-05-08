#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{

class VISO_EXPORT ClearColor
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