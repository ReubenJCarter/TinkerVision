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
	
class TINKERVISION_EXPORT Flip
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Direction{HORIZONTAL, VERTICAL}; 

		Flip(); 
        ~Flip(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetDirection(bool flipHor, bool flipVer);
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Flip: public BaseProcess1In1Out<Process::Flip>
{
    TNKRVIS_CLONEABLE_MACRO(Flip) 
    public:
        void SetParams() 
        { 
            proc.SetDirection( GetInputData(2).DerefAsType<bool>(BoolData, false),
                               GetInputData(3).DerefAsType<bool>(BoolData, false) ); 
        }
}; 

}
}

}