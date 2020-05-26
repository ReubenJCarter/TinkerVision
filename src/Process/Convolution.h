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

class TINKERVISION_EXPORT Convolution
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Convolution(); 
        ~Convolution(); 
		void Run(ImageGPU* input, ImageGPU* conv, ImageGPU* output); 
        void Run(Image* input, Image* conv, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Convolution:  public Node
{
    TNKRVIS_CLONEABLE_MACRO(Convolution) 
    private:
        Data outImageData;
        Process::Convolution proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {

             Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);
            ImageGPU* inSrcAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* convAsimage = GetInputData(2).AsType<Image>(ImageData);  
            ImageGPU* convAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

           
                        
            if(inSrcAsimage != NULL && convAsimage != NULL && dstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, convAsimage, dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(inSrcAsimageGPU != NULL && convAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                proc.Run(inSrcAsimageGPU, convAsimageGPU, dstAsimageGPU);
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