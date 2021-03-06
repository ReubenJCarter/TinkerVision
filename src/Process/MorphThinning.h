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
	
class TINKERVISION_EXPORT MorphThinning
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MorphThinning(); 
        ~MorphThinning(); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class MorphThinning: public BaseProcess1In1OutCPUOnly<Process::MorphThinning>
{
    TNKRVIS_CLONEABLE_MACRO(MorphThinning) 
    public:
        void SetParams() { }
}; 

}
}

}