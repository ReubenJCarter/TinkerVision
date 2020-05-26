#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

#include "../ComputeGraph/Node.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT HighLowThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		HighLowThreshold(); 
        ~HighLowThreshold(); 
        void SetLowThreshold(float t);
        void SetHighThreshold(float t);
        void SetLowThreshold(Color t);
        void SetHighThreshold(Color t);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class HighLowThreshold: public BaseProcess1In1Out<Process::HighLowThreshold>
{
    TNKRVIS_CLONEABLE_MACRO(HighLowThreshold) 
    public:
        void SetParams() 
        {            
            proc.SetLowThreshold( GetInputData(2).DerefAsType<Color>(ColorData, Color(0, 0, 0) ) ); 
            proc.SetHighThreshold( GetInputData(3).DerefAsType<Color>(ColorData, Color(1, 1, 1) ) ); 
        }
};  

}
}

}