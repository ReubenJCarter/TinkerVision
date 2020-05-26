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

class TINKERVISION_EXPORT Downsample
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum Mode {NEAREST, BOX, BILINEAR};

		Downsample(); 
        ~Downsample(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetMode(Mode m); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Downsample: public BaseProcess1In1Out<Process::Downsample>
{
    TNKRVIS_CLONEABLE_MACRO(Downsample) 
    public:
        void SetParams() 
        {
            proc.SetMode( (Process::Downsample::Mode)GetInputData(2).DerefAsType<int>(IntData, 0) ); 
        }
};

}
}

}