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

class TINKERVISION_EXPORT ClearColor
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ClearColor(); 
        ~ClearColor(); 
        void SetColor(float r, float g, float b, float a=1); 
		void Run(ImageGPU* output); 
        void Run(Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class ClearColor: public Node
{
    TNKRVIS_CLONEABLE_MACRO(ClearColor) 
    private:
        Data outImageData;
        Process::ClearColor proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetColor(GetInputData(1).DerefAsType<float>(FloatData, 0), 
                          GetInputData(2).DerefAsType<float>(FloatData, 0), 
                          GetInputData(3).DerefAsType<float>(FloatData, 0),
                          GetInputData(4).DerefAsType<float>(FloatData, 1)); 

            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
                        
            if(dstAsimage != NULL)
            {
                proc.Run( dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(dstAsimageGPU != NULL)
            {
                proc.Run( dstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
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