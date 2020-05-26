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
	
class TINKERVISION_EXPORT CameraDistortion
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CameraDistortion(); 
        ~CameraDistortion(); 
        void SetRadialCoefs(float k0, float k1, float k2);
        void SetTangentialCoefs(float p0, float p1);
        void SetFocalLength(float fl);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class CameraDistortion: public BaseProcess1In1Out<Process::CameraDistortion>
{
    TNKRVIS_CLONEABLE_MACRO(CameraDistortion) 
    public:
        void SetParams() 
        {
            proc.SetRadialCoefs( GetInputData(2).DerefAsType<float>(FloatData, 0), 
                                 GetInputData(3).DerefAsType<float>(FloatData, 0), 
                                 GetInputData(4).DerefAsType<float>(FloatData, 0) );
            proc.SetTangentialCoefs( GetInputData(5).DerefAsType<float>(FloatData, 0),
                                     GetInputData(6).DerefAsType<float>(FloatData, 0) );
            proc.SetFocalLength( GetInputData(7).DerefAsType<float>(FloatData, 1) );
        }
};  

}
}

}