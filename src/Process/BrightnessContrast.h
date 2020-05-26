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

class TINKERVISION_EXPORT BrightnessContrast
{
    private: 
        class Internal;
        Internal* internal;

	public:
		BrightnessContrast(); 
        ~BrightnessContrast(); 
        void SetBrightness(float b); 
        void SetContrast(float c); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class BrightnessContrast: public BaseProcess1In1Out<Process::BrightnessContrast>
{
    TNKRVIS_CLONEABLE_MACRO(BrightnessContrast) 
    public:
        void SetParams() 
        {
            proc.SetBrightness( GetInputData(2).DerefAsType<float>(FloatData, 0) );
            proc.SetContrast( GetInputData(3).DerefAsType<float>(FloatData, 1) );
        }
}; 

}
}

}