#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT Thinning
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Thinning(); 
        ~Thinning(); 
        void Run(Image* input, Image* output); 
};
	
}
}