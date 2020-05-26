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
	
class TINKERVISION_EXPORT HSVToRGB
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HSVToRGB(); 
        ~HSVToRGB(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class HSVToRGB: public BaseProcess1In1Out<Process::HSVToRGB>
{
    TNKRVIS_CLONEABLE_MACRO(HSVToRGB) 
    public:
        void SetParams() {}
}; 

}
}

}