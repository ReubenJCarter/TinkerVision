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
	
class TINKERVISION_EXPORT StereoMatchSAD
{
    private: 
        class Internal;
        Internal* internal;

	public:
		StereoMatchSAD(); 
        ~StereoMatchSAD(); 
		void Run(ImageGPU* inputL, ImageGPU* inputR, ImageGPU* output); 
        void Run(Image* inputL, Image* inputR, Image* output); 
        void SetBlockSize(int W, int H);
        void SetMaxK(int mk);
        void SetSADThreshold(float st); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class StereoMatchSAD: public Node
{
    TNKRVIS_CLONEABLE_MACRO(StereoMatchSAD) 
    private:
        Data outImageData;
        Process::StereoMatchSAD proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {

            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcLAsimage = GetInputData(1).AsType<Image>(ImageData);
            ImageGPU* inSrcLAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcRAsimage = GetInputData(2).AsType<Image>(ImageData);
            ImageGPU* inSrcRAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

            proc.SetBlockSize( GetInputData(3).DerefAsType<int>(IntData, 32),
                               GetInputData(4).DerefAsType<int>(IntData, 32));
            proc.SetMaxK( GetInputData(5).DerefAsType<int>(IntData, 80));
            proc.SetSADThreshold( GetInputData(6).DerefAsType<float>(FloatData, 50)); 
                        
            if(inSrcLAsimage != NULL && inSrcRAsimage != NULL && dstAsimage != NULL)
            {
                proc.Run(inSrcLAsimage, inSrcRAsimage, dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(inSrcLAsimageGPU != NULL && inSrcRAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                proc.Run(inSrcLAsimageGPU, inSrcRAsimageGPU, dstAsimageGPU);
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