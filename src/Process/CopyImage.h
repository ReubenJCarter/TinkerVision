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
	
class TINKERVISION_EXPORT CopyImage
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CopyImage(); 
        ~CopyImage(); 
        void SetFormatTranslate(bool ftr); 
        void UseOutputSize(bool outSz); 
        void SetOffset(int x, int y); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class CopyImage: public BaseProcess1In1Out<Process::CopyImage>
{
    TNKRVIS_CLONEABLE_MACRO(CopyImage) 
    public:
        void SetParams() 
        {
            proc.SetFormatTranslate(GetInputData(2).DerefAsType<bool>(BoolData, true)); 

            proc.UseOutputSize(GetInputData(3).DerefAsType<bool>(BoolData, false)); 

            proc.SetOffset(GetInputData(4).DerefAsType<int>(IntData, 0),
                           GetInputData(5).DerefAsType<int>(IntData, 0)); 
        }
};  

}
}

}