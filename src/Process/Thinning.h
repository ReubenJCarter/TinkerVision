#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT Thinning
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