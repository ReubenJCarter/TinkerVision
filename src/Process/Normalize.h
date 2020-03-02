#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT Normalize
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Normalize(); 
        ~Normalize(); 
        void Run(Image* input, Image* output); 
};
	
}
}