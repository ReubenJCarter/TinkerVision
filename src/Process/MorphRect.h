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
	
class TINKERVISION_EXPORT MorphRect
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode{ERODE, DILATE};

		MorphRect(); 
        ~MorphRect(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 

        void SetMode(Mode mode);
        void SetKernel(int w, int h);  
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class MorphRect: public BaseProcess1In1Out<Process::MorphRect>
{
    TNKRVIS_CLONEABLE_MACRO(MorphRect) 
    public:
        void SetParams() 
        {
            proc.SetMode( (Process::MorphRect::Mode)GetInputData(2).DerefAsType<int>(IntData, (int)Process::MorphRect::Mode::DILATE) );
            proc.SetKernel( GetInputData(3).DerefAsType<int>(IntData, 3),
                            GetInputData(4).DerefAsType<int>(IntData, 3) );  
        }
}; 

}
}

}