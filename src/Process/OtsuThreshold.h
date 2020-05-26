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
	
class TINKERVISION_EXPORT OtsuThreshold
{
    private: 
        class Internal;
        Internal* internal;

	public:
		OtsuThreshold(); 
        ~OtsuThreshold(); 
        float Run(Image* input); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class OtsuThreshold: public Node //TODO (with array support)
{
    TNKRVIS_CLONEABLE_MACRO(OtsuThreshold) 
    private:
        float threshold; 
        Data outThresholdData;
        Process::OtsuThreshold proc; 

    public:
        Data GetOutput(int inx){ return outThresholdData; }

        void Run()
        {
            Image* srcAsimage = GetInputData(0).AsType<Image>(ImageData);  
                        
            if(srcAsimage != NULL)
            {
                threshold = proc.Run( srcAsimage);
                outThresholdData = Data(FloatData, &threshold); 
            }
            else
            {
                outThresholdData = Data(NullData, NULL); 
            }
        }
};

}
}

}