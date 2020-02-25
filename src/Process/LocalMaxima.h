#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

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