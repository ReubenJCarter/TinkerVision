#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include <vector>

namespace TnkrVis
{
namespace Process
{

class TINKERVISION_EXPORT MarkerBitExtract
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MarkerBitExtract(); 
        ~MarkerBitExtract(); 
        void SetGridSize(int W, int H); 
        void SetMarginSize(float s); 
        void Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages); 
};
	
}
}