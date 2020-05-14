#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT CopyImage
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