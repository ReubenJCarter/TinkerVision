#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

#include "../ComputeGraph/Node.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT Threshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Threshold(); 
        ~Threshold(); 
        void SetThreshold(float t);
        void SetThreshold(Color t);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Threshold: public BaseProcess1In1Out<Process::Threshold>
{
    TNKRVIS_CLONEABLE_MACRO(Threshold) 
    public:
        void SetParams() 
        { 
            proc.SetThreshold( GetInputData(2).DerefAsType<Color>(ColorData, Color(0.5, 0.5, 0.5)));
        }
};

}
}

}