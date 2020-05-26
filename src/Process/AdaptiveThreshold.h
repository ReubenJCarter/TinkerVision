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
	
class TINKERVISION_EXPORT AdaptiveThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		AdaptiveThreshold(); 
        ~AdaptiveThreshold(); 
        void SetThreshold(float t);
        void SetSize(int s); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class AdaptiveThreshold: public BaseProcess1In1Out<Process::AdaptiveThreshold>
{
    TNKRVIS_CLONEABLE_MACRO(AdaptiveThreshold) 
    public:
        void SetParams() 
        {
            proc.SetThreshold( GetInputData(2).DerefAsType<float>(FloatData, 0.5f) );
            proc.SetSize( GetInputData(3).DerefAsType<int>(IntData, 7) );
        }
};

}
}

}