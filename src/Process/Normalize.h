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
	
class TINKERVISION_EXPORT Normalize
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Normalize(); 
        ~Normalize(); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Normalize: public BaseProcess1In1OutCPUOnly<Process::Normalize>
{
    TNKRVIS_CLONEABLE_MACRO(Normalize) 
    public:
        void SetParams() { }
}; 

}
}

}