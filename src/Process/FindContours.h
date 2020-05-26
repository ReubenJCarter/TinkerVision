#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include "../ComputeGraph/Node.h"

#include <vector>

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT FindContours
{
    private: 
        class Internal;
        Internal* internal;

	public:
		FindContours(); 
        ~FindContours(); 
        void Run(Image* input, Image* output, std::vector<Contour>* contours=NULL); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class FindContours: public BaseProcess1In1OutCPUOnly<Process::FindContours>
{
    TNKRVIS_CLONEABLE_MACRO(FindContours) 
    public:
        void SetParams() {}
};

}
}

}