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
	
class TINKERVISION_EXPORT StructureTensor
{
    private: 
        class Internal;
        Internal* internal;

	public:
		StructureTensor(); 
        ~StructureTensor(); 
        
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class StructureTensor: public BaseProcess1In1Out<Process::StructureTensor>
{
    TNKRVIS_CLONEABLE_MACRO(StructureTensor) 
    public:
        void SetParams() { }
}; 

}
}

}