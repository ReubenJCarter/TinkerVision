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
	
class TINKERVISION_EXPORT GaussianBlur
{
    private: 
        class Internal;
        Internal* internal;

	public:
		GaussianBlur(); 
        ~GaussianBlur(); 
        void SetSigma(float s); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class GaussianBlur: public BaseProcess1In1Out<Process::GaussianBlur>
{
    TNKRVIS_CLONEABLE_MACRO(GaussianBlur) 
    public:
        void SetParams() 
        {
            proc.SetSigma( GetInputData(2).DerefAsType<int>(FloatData, 1.6f) ); 
        }
};

}
}

}