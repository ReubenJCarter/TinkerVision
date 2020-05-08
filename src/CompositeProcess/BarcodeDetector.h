#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include <vector>

namespace Viso
{
namespace CompositeProcess
{
	
class VISO_EXPORT BarcodeDetector
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BarcodeDetector(); 
        ~BarcodeDetector(); 
        
		void Run(ImageGPU* input, std::vector<Viso::Contour>* barcodeQuads, std::vector< std::vector<unsigned char> >* barcodeData);  
};
	
}
}