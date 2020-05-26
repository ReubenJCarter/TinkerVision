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
	
class TINKERVISION_EXPORT GaussianDerivative
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Direction{HORIZONTAL, VERTICAL}; 

		GaussianDerivative(); 
        ~GaussianDerivative(); 
        void SetSigma(float s); 
        void SetDirection(Direction dir); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class GaussianDerivative: public BaseProcess1In1Out<Process::GaussianDerivative>
{
    TNKRVIS_CLONEABLE_MACRO(GaussianDerivative) 
    public:
        void SetParams() 
        {
            proc.SetSigma( GetInputData(2).DerefAsType<int>(FloatData, 1.6f) ); 
            proc.SetDirection( (Process::GaussianDerivative::Direction)GetInputData(3).DerefAsType<int>(IntData, (int)Process::GaussianDerivative::Direction::HORIZONTAL ) ); 
        }
};

}
}

}