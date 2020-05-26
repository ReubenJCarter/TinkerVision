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
	
class TINKERVISION_EXPORT ChannelDemux
{
    private: 
        class Internal;
        Internal* internal;
        
	public:
		ChannelDemux(); 
        ~ChannelDemux(); 
        void SetChannel(int ch); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class ChannelDemux: public BaseProcess1In1Out<Process::ChannelDemux>
{
    TNKRVIS_CLONEABLE_MACRO(ChannelDemux) 
    public:
        void SetParams() 
        {
            proc.SetChannel( GetInputData(2).DerefAsType<int>(IntData, 0));
        }
}; 

}
}

}