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
	
class TINKERVISION_EXPORT BilateralFilter
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BilateralFilter(); 
        ~BilateralFilter(); 
        void SetSigma(float s);
        void SetSize(int s); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class BilateralFilter: public BaseProcess1In1Out<Process::BilateralFilter>
{
    TNKRVIS_CLONEABLE_MACRO(BilateralFilter) 
    public:
        void SetParams() 
        {
            proc.SetSigma( GetInputData(2).DerefAsType<float>(FloatData, 1.6));
            proc.SetSize( GetInputData(2).DerefAsType<int>(IntData, 5));
        }
}; 

}
}

}