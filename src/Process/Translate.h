#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT Translate
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