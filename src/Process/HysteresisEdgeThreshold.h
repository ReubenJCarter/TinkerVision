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
	
class TINKERVISION_EXPORT HysteresisEdgeThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HysteresisEdgeThreshold(); 
        ~HysteresisEdgeThreshold(); 
        void Run(Image* input, Image* output); 
        void SetHighThreshold(float highThr);
        void SetLowThreshold(float lowThr); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class HysteresisEdgeThreshold: public BaseProcess1In1OutCPUOnly<Process::HysteresisEdgeThreshold>
{
    TNKRVIS_CLONEABLE_MACRO(HysteresisEdgeThreshold) 
    public:
        void SetParams() 
        {
            proc.SetHighThreshold( GetInputData(2).DerefAsType<float>(FloatData, 1.0f ) ); 
            proc.SetLowThreshold( GetInputData(3).DerefAsType<float>(FloatData, 0.0f) ); 
        }
}; 

}
}

}