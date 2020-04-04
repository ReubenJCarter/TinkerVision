#pragma once

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"

namespace Visi
{
namespace ComputeGraph
{
namespace Nodes
{

class ImageAllocate: public Node
{
    VISI_CLONEABLE_MACRO(ImageAllocate) 
    private:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/

	public:
        Data GetOutput(int inx){ return outImageData; }

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
                outImageData = Data(ImageData, inAsimage);
            }
            else if(inAsimageGPU != NULL)
            {
                inAsimageGPU->Allocate(inWidth, inHeight, inType); 
                outImageData = Data(ImageGPUData, inAsimageGPU);
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
};

class ImageDeallocate: public Node
{
    VISI_CLONEABLE_MACRO(ImageDeallocate) 
    private:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/

	public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* inAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
        
            if(inAsimage != NULL)
            {
                inAsimage->Deallocate(); 
                outImageData = Data(ImageData, inAsimage);
            }
            else if(inAsimageGPU != NULL)
            {
                inAsimageGPU->Deallocate(); 
                outImageData = Data(ImageGPUData, inAsimageGPU);
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
};

class ImageGetDims: public Node
{
    VISI_CLONEABLE_MACRO(ImageGetDims) 
    private:
        Data outIntData; 
        int width;
        int height;  
        ImageType imtype;
        
	public:
        Data GetOutput(int inx){ return outIntData; }

        void Run()
        {
            Image* inAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
        
            if(inAsimage != NULL)
            {
                width = inAsimage->GetWidth(); 
                height = inAsimage->GetHeight(); 
                imtype = inAsimage->GetType(); 
                outIntData = Data(IntData, &width);
            }
            else if(inAsimageGPU != NULL)
            {
                width = inAsimageGPU->GetWidth();
                height = inAsimage->GetHeight(); 
                imtype = inAsimage->GetType();  
                outIntData = Data(IntData, &width);
            }
            else
            {
                outIntData = Data(NullData, NULL);
            }
        }
};
	
class ImageCopy: public Node
{
    VISI_CLONEABLE_MACRO(ImageCopy) 
    private:
        Data outImageData;  
        
	public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* inAsimageDst = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPUDst = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inAsimageSrc = GetInputData(1).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPUSrc = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  
        
            if(inAsimageDst != NULL && inAsimageGPUSrc != NULL)
            {
                inAsimageDst->Copy(inAsimageGPUSrc);
                outImageData = Data(ImageData, inAsimageDst);
            }
            else if(inAsimageGPUDst != NULL && inAsimageSrc != NULL)
            {
                inAsimageGPUDst->Copy(inAsimageSrc);
                outImageData = Data(ImageGPUData, inAsimageGPUDst);
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