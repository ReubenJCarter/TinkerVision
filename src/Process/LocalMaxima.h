#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/VectorMath.h"

#include "../ComputeGraph/Node.h"

#include <vector>

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT LocalMaxima
{
    private: 
        class Internal;
        Internal* internal;

	public:
		LocalMaxima(); 
        ~LocalMaxima(); 
        void Run(Image* input, Image* output, std::vector<Vec2>& localMaxima=std::vector<Vec2>()); 
        void SetThreshold(float t); 
        void SetSize(int s);
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class LocalMaxima: public BaseProcess1In1OutCPUOnly<Process::LocalMaxima>
{
    TNKRVIS_CLONEABLE_MACRO(LocalMaxima) 
    public:
        void SetParams() 
        { //TODO Array of centers support (needs general array support)
            proc.SetThreshold( GetInputData(2).DerefAsType<float>(FloatData, 1.0f )  ); 
            proc.SetSize( GetInputData(3).DerefAsType<float>(FloatData, 1.0f )  );
        }
};

}
}

}