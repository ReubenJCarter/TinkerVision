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

class TINKERVISION_EXPORT MarkerBitExtract
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MarkerBitExtract(); 
        ~MarkerBitExtract(); 
        void SetGridSize(int W, int H); 
        void SetMarginSize(float s); 
        void Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class MarkerBitExtract: public Node //TODO (with array support)
{
    TNKRVIS_CLONEABLE_MACRO(MarkerBitExtract) 
    private:
        Data outImageData;
        Process::MarkerBitExtract proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetGridSize(GetInputData(3).DerefAsType<int>(IntData, 0), 
                             GetInputData(4).DerefAsType<int>(IntData, 0)); 
            proc.SetMarginSize( GetInputData(5).DerefAsType<float>(FloatData, 0)); 

            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
                        
            if(dstAsimage != NULL)
            {
                //proc.Run( dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
};

}
}

}