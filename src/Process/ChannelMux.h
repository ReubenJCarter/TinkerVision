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
	
class TINKERVISION_EXPORT ChannelMux
{
    private: 
        class Internal;
        Internal* internal;
        
	public:
		ChannelMux(); 
        ~ChannelMux(); 
        void SetChannel(int ch); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class ChannelMux: public BaseProcess1In1Out<Process::ChannelMux>
{
    TNKRVIS_CLONEABLE_MACRO(ChannelMux) 
    public:
        void SetParams() 
        {
            proc.SetChannel( GetInputData(2).DerefAsType<int>(IntData, 0) );
        }
}; 

}
}

}