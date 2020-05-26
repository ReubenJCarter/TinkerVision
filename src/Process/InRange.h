#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

#include "../ComputeGraph/Node.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT InRange
{
    private: 
        class Internal;
        Internal* internal;

	public:
		InRange(); 
        ~InRange(); 
        void SetLowThreshold(float t);
        void SetHighThreshold(float t);
        void SetLowThreshold(Vec3 t);
        void SetHighThreshold(Vec3 t);
        void SetInvertRange(bool invCh0, bool invCh1, bool invCh2); 
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class InRange: public BaseProcess1In1Out<Process::InRange>
{
    TNKRVIS_CLONEABLE_MACRO(InRange) 
    public:
        void SetParams() 
        {
            proc.SetHighThreshold( GetInputData(2).DerefAsType<Vec3>(Vec3Data, Vec3(1, 1, 1) ) ); 
            proc.SetLowThreshold( GetInputData(3).DerefAsType<Vec3>(Vec3Data, Vec3(0, 0, 0)) ); 

            proc.SetInvertRange( GetInputData(4).DerefAsType<bool>(BoolData, false),
                                 GetInputData(5).DerefAsType<bool>(BoolData, false),
                                 GetInputData(6).DerefAsType<bool>(BoolData, false)); 
        }
}; 

}
}

}