#pragma once

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"
#include "../../IO/ImageFile.h"

namespace Viso
{
namespace ComputeGraph
{
namespace Nodes
{

class ImageFileRead: public Node
{
    VISO_CLONEABLE_MACRO(ImageFileRead) 
    private:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/

	public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* inAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
            std::string inFileName = GetInputData(1).DerefAsType<std::string>(StringData, "");            
            
            if(inAsimage != NULL)
            {
                Viso::IO::ImageFile::Read(inFileName, inAsimage);
                outImageData = Data(ImageData, inAsimage);
            }
            else if(inAsimageGPU != NULL)
            {
                Viso::IO::ImageFile::Read(inFileName, inAsimageGPU);
                outImageData = Data(ImageGPUData, inAsimageGPU);
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
};

class ImageFileWrite: public Node
{
    VISO_CLONEABLE_MACRO(ImageFileWrite) 
    private:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/

	public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* inAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* inAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
            std::string inFileName = GetInputData(1).DerefAsType<std::string>(StringData, "");            
            
            if(inAsimage != NULL)
            {
                Viso::IO::ImageFile::Write(inFileName, inAsimage);
                outImageData = Data(ImageData, inAsimage);
            }
            else if(inAsimageGPU != NULL)
            {
                Viso::IO::ImageFile::Write(inFileName, inAsimageGPU);
                outImageData = Data(ImageGPUData, inAsimageGPU);
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