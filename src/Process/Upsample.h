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

class TINKERVISION_EXPORT Upsample
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode {NEAREST, BILINEAR};

		Upsample(); 
        ~Upsample(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetMode(Mode m); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Upsample: public BaseProcess1In1Out<Process::Upsample>
{
    TNKRVIS_CLONEABLE_MACRO(Upsample) 
    public:
        void SetParams() 
        { 
            proc.SetMode( (Process::Upsample::Mode)GetInputData(2).DerefAsType<int>(IntData, (int)Process::Upsample::Mode::BILINEAR ));
        }
};

}
}

}