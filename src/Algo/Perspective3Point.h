#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

#include <vector>

namespace TnkrVis
{
namespace Algo
{

class TINKERVISION_EXPORT Perspective3Point
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
}