#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"
#include "VectorMath.h"

namespace Visi
{
	
class VISI_EXPORT InRange
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
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}