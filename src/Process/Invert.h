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
	
class TINKERVISION_EXPORT Invert
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Invert(); 
        ~Invert(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Invert: public BaseProcess1In1Out<Process::Invert>
{
    TNKRVIS_CLONEABLE_MACRO(Invert) 
    public:
        void SetParams() {}
}; 

}
}

}