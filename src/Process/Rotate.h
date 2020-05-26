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
	
class TINKERVISION_EXPORT Rotate
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Rotate(); 
        ~Rotate(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetRotation(double degs);
        void SetResizeToFit(bool r); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Rotate: public BaseProcess1In1Out<Process::Rotate>
{
    TNKRVIS_CLONEABLE_MACRO(Rotate) 
    public:
        void SetParams() 
        { 
            proc.SetRotation( GetInputData(2).DerefAsType<float>(FloatData, 0));
            proc.SetResizeToFit( GetInputData(3).DerefAsType<bool>(BoolData, true)); 
        }
}; 

}
}

}