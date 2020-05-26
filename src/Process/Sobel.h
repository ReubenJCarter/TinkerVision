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
	
class TINKERVISION_EXPORT Sobel
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode{FULL, GRAD_ONLY, MAG_ONLY};

		Sobel(); 
        ~Sobel(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetMode(Mode m); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Sobel: public BaseProcess1In1Out<Process::Sobel>
{
    TNKRVIS_CLONEABLE_MACRO(Sobel) 
    public:
        void SetParams() 
        { 
            proc.SetMode( (Process::Sobel::Mode)GetInputData(2).DerefAsType<int>(IntData, Process::Sobel::Mode::FULL));
        }
}; 

}
}

}