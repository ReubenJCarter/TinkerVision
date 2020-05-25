#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT MorphThinning
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MorphThinning(); 
        ~MorphThinning(); 
        void Run(Image* input, Image* output); 
};
	
}
}