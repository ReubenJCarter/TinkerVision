#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT CopyImage
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CopyImage(); 
        ~CopyImage(); 
        void SetFormatTranslate(bool ftr); 
        void UseOutputSize(bool outSz); 
        void SetOffset(int x, int y); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}