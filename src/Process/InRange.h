#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT InRange
{
    private: 
        class Internal;
        Internal* internal;

	public:
		InRange(); 
        ~InRange(); 
        void SetLowThreshold(float t);
        void SetHighThreshold(float t);
        void SetLowThreshold(Vec3 t);
        void SetHighThreshold(Vec3 t);
        void SetInvertRange(bool invCh0, bool invCh1, bool invCh2); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}
}