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
	
class TINKERVISION_EXPORT Translate
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Translate(); 
        ~Translate(); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
        void SetTranslation(float x, float y);
        void SetResizeToFit(bool r); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Translate: public BaseProcess1In1Out<Process::Translate>
{
    TNKRVIS_CLONEABLE_MACRO(Translate) 
    public:
        void SetParams() 
        { 
            proc.SetTranslation( GetInputData(2).DerefAsType<float>(FloatData, 0), 
                                 GetInputData(3).DerefAsType<float>(FloatData, 0));
            proc.SetResizeToFit( GetInputData(4).DerefAsType<bool>(BoolData, true));
        }
}; 

}
}

}