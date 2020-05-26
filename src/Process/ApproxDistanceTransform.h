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
	
class TINKERVISION_EXPORT ApproxDistanceTransform
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ApproxDistanceTransform(); 
        ~ApproxDistanceTransform(); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class ApproxDistanceTransform: public BaseProcess1In1OutCPUOnly<Process::ApproxDistanceTransform>
{
    TNKRVIS_CLONEABLE_MACRO(ApproxDistanceTransform) 
}; 

}
}

}