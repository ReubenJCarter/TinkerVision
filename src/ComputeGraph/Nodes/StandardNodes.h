#pragma once

#include "Visi_export.h"

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"

namespace Visi
{

namespace ComputeGraph
{

namespace Nodes
{

class VISI_EXPORT AllocateImage: public Node
{
    private:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/

	public:
        Data GetOutput(int inx)
        {
            return outImageData; 
        }

        void Run()
        {
            Image* inAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
            int inWidth = GetInputData(1).DerefAsType<int>(IntData, 0);            
            int inHeight = GetInputData(2).DerefAsType<int>(IntData, 0);         
            ImageType inType = GetInputData(3).DerefAsType<ImageType>(ImageTypeData, ImageType::RGB8);        
            
            if(inAsimage != NULL)
            {
                inAsimage->Allocate(inWidth, inHeight, inType); 
                outImageData = {ImageData, inAsimage};
            }
            else if(inAsimageGPU != NULL)
            {
                inAsimageGPU->Allocate(inWidth, inHeight, inType); 
                outImageData = {ImageGPUData, inAsimageGPU};
            }
            else
            {
                outImageData = {NullDate, NULL}; 
            }
        }
};
	
}

}

}