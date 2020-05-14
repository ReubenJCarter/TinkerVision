#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include <vector>

namespace TnkrVis
{
namespace CompositeProcess
{
	
class TINKERVISION_EXPORT BarcodeDetector
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BarcodeDetector(); 
        ~BarcodeDetector(); 
        
		void Run(ImageGPU* input, std::vector<TnkrVis::Contour>* barcodeQuads, std::vector< std::vector<unsigned char> >* barcodeData);  
};
	
}
}