#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT Translate
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Translate(); 
        ~Translate(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetTranslation(float x, float y);
        void SetResizeToFit(bool r); 
};
	
}
}