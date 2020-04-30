#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include <vector>

namespace Visi
{
namespace CompositeProcess
{
	
class VISI_EXPORT BarcodeDetector
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BarcodeDetector(); 
        ~BarcodeDetector(); 
        
		void Run(ImageGPU* input, std::vector<Visi::Contour>* barcodeQuads, std::vector< std::vector<unsigned char> >* barcodeData);  
};
	
}
}