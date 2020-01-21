#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"
#include "VectorMath.h"

#include <vector>

namespace Visi
{
	
class VISI_EXPORT LocalMaxima
{
    private: 
        class Internal;
        Internal* internal;

	public:
		LocalMaxima(); 
        ~LocalMaxima(); 
        void Run(Image* input, Image* output, std::vector<Vec2>& localMaxima=std::vector<Vec2>()); 
        void SetThreshold(float t); 
        void SetSize(int s);
};
	
}