#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../ComputeGraph/Node.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT HarrisShiTomasiResponse
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HarrisShiTomasiResponse(); 
        ~HarrisShiTomasiResponse(); 
        
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class HarrisShiTomasiResponse: public BaseProcess1In1Out<Process::HarrisShiTomasiResponse>
{
    TNKRVIS_CLONEABLE_MACRO(HarrisShiTomasiResponse) 
    public:
        void SetParams() {}
}; 

}
}

}