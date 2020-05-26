#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../ComputeGraph/Node.h"

#include <vector>

namespace TnkrVis
{
namespace Process
{

class TINKERVISION_EXPORT Mipmaps
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Mipmaps(); 
        ~Mipmaps(); 
		void Run(ImageGPU* input, std::vector<ImageGPU>* output); 
        void Run(Image* input, std::vector<Image>* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class Mipmaps: public Node //TODO (with array support)
{
    TNKRVIS_CLONEABLE_MACRO(Mipmaps) 
    private:
        Data outImageData;
        Process::Mipmaps proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
                        
            if(dstAsimage != NULL)
            {
                //proc.Run( dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(dstAsimageGPU != NULL)
            {
                //proc.Run( dstAsimageGPU);
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