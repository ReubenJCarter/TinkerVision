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
	
class TINKERVISION_EXPORT MedianFilter
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MedianFilter(); 
        ~MedianFilter(); 
        void SetSize(int s); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class MedianFilter: public BaseProcess1In1Out<Process::MedianFilter>
{
    TNKRVIS_CLONEABLE_MACRO(MedianFilter) 
    public:
        void SetParams() 
        {
            proc.SetSize( GetInputData(2).DerefAsType<int>(IntData, 3 ) ); 
        }
};

}
}

}