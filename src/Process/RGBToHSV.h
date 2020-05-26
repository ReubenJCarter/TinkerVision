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
	
class TINKERVISION_EXPORT RGBToHSV
{
    private: 
        class Internal;
        Internal* internal;

	public:
		RGBToHSV(); 
        ~RGBToHSV(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class RGBToHSV: public BaseProcess1In1Out<Process::RGBToHSV>
{
    TNKRVIS_CLONEABLE_MACRO(RGBToHSV) 
    public:
        void SetParams() { }
}; 

}
}

}