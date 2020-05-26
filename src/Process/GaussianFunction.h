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

class TINKERVISION_EXPORT GaussianFunction
{
    private: 
        class Internal;
        Internal* internal;

	public:
        enum GenerateMode{ FIXED, SIZE_TO_SIGMA, SIGMA_TO_SIZE };

		GaussianFunction(); 
        ~GaussianFunction(); 
        void SetSigma(float sig); 
        void SetSigma(float sigX, float sigY); 
        void SetGenerateMode(GenerateMode gm); 
		void Run(ImageGPU* dst); 
        void Run(Image* dst); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class GaussianFunction: public Node
{
    TNKRVIS_CLONEABLE_MACRO(GaussianFunction) 
    private:
        Data outImageData;
        Process::GaussianFunction proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetSigma(GetInputData(1).DerefAsType<float>(FloatData, 0), 
                          GetInputData(2).DerefAsType<float>(FloatData, 0)); 
            proc.SetGenerateMode( (Process::GaussianFunction::GenerateMode)GetInputData(3).DerefAsType<int>(IntData, 0)); 

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