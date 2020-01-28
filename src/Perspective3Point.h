#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"
#include "VectorMath.h"

#include <vector>

namespace Visi
{

class VISI_EXPORT Perspective3Point
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Perspective3Point(); 
        ~Perspective3Point();
        bool Run(std::vector<Vec3>* normalizedImageCoords, std::vector<Vec3>* worldCoords, std::vector<CameraPose>* cameraPoses);  
};
	
}