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
	
class TINKERVISION_EXPORT NonMaximumEdgeSuppression
{
    private: 
        class Internal;
        Internal* internal;

	public:
		NonMaximumEdgeSuppression(); 
        ~NonMaximumEdgeSuppression(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class NonMaximumEdgeSuppression: public BaseProcess1In1Out<Process::NonMaximumEdgeSuppression>
{
    TNKRVIS_CLONEABLE_MACRO(NonMaximumEdgeSuppression) 
    public:
        void SetParams() { }
}; 

}
}

}