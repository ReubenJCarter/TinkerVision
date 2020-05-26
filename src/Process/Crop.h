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

class TINKERVISION_EXPORT Crop
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Crop(); 
        ~Crop(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetOffset(int x, int y); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Crop: public BaseProcess1In1Out<Process::Crop>
{
    TNKRVIS_CLONEABLE_MACRO(Crop) 
    public:
        void SetParams() 
        {
            proc.SetOffset(GetInputData(2).DerefAsType<int>(IntData, 0),
                           GetInputData(3).DerefAsType<int>(IntData, 0)); 
        }
}; 

}
}

}