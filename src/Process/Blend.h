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
	
class TINKERVISION_EXPORT Blend
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum BlendMode{BLEND_ADD, BLEND_SUBTRACT, BLEND_DIFFERENCE, BLEND_DIVIDE, BLEND_MULTIPLY, BLEND_OVERLAY};

		Blend(); 
        ~Blend(); 

        void SetMode(BlendMode bm);

		void Run(ImageGPU* inputTop, ImageGPU* inputBottom, ImageGPU* output, ImageGPU* blendMask); 
        void Run(Image* inputTop, Image* inputBottom, Image* output, Image* blendMask); 

        void Run(ImageGPU* inputTop, ImageGPU* inputBottom, ImageGPU* output); 
        void Run(Image* inputTop, Image* inputBottom, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Blend: public Node
{
    TNKRVIS_CLONEABLE_MACRO(Blend) 
    private:
        Data outImageData;
        Process::Blend proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetMode( (Process::Blend::BlendMode)GetInputData(4).DerefAsType<int>(IntData, 0) ); 

            Image* bottomAsimage = GetInputData(0).AsType<Image>(ImageData);
            ImageGPU* bottomAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* topAsimage = GetInputData(1).AsType<Image>(ImageData);  
            ImageGPU* topAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* dstAsimage = GetInputData(2).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

            Image* maskAsimage = GetInputData(3).AsType<Image>(ImageData);  
            ImageGPU* maskAsimageGPU = GetInputData(3).AsType<ImageGPU>(ImageGPUData);  
                        
            if(bottomAsimage != NULL && topAsimage != NULL && dstAsimage != NULL)
            {
                if(maskAsimage != NULL)
                {
                    proc.Run(topAsimage, bottomAsimage, dstAsimage, maskAsimage);
                    outImageData = Data(DataType::ImageData, dstAsimage); 
                }
                else
                {
                    proc.Run(topAsimage, bottomAsimage, dstAsimage);
                    outImageData = Data(DataType::ImageData, dstAsimage); 
                }
            }
            else if(bottomAsimageGPU != NULL && topAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                if(maskAsimageGPU != NULL)
                {
                    proc.Run(topAsimageGPU, bottomAsimageGPU, dstAsimageGPU, maskAsimageGPU);
                    outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
                }
                else
                {
                    proc.Run(topAsimageGPU, bottomAsimageGPU, dstAsimageGPU);
                    outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
                }
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