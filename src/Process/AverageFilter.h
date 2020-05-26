#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

#include "../ComputeGraph/Node.h"

#include <vector>

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT AverageFilter
{
    private: 
        class Internal;
        Internal* internal;

	public:
		AverageFilter(); 
        ~AverageFilter(); 
        void Run(Image* input, Image* output); 
        void Run(ImageGPU* input, ImageGPU* output); 
        void SetSize(int s);
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class AverageFilter: public BaseProcess1In1Out<Process::AverageFilter>
{
    TNKRVIS_CLONEABLE_MACRO(AverageFilter) 
    public:
        void SetParams() 
        {
            proc.SetSize( GetInputData(2).DerefAsType<int>(IntData, 0) );
        }
};  

}
}

}