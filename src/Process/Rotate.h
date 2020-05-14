#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT Rotate
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Rotate(); 
        ~Rotate(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetRotation(double degs);
        void SetResizeToFit(bool r); 
};
	
}
}