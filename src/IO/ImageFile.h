#pragma once

#include <string>

#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../ComputeGraph/Node.h"

namespace TnkrVis
{
namespace IO
{

class TINKERVISION_EXPORT ImageFile
{	
    public:
        static bool Write(std::string fileName, Image* image);
        static bool Write(std::string fileName, ImageGPU* image);
        static bool Read(std::string fileName, Image* image); 
        static bool Read(std::string fileName, ImageGPU* image); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

class ImageFileRead: public Node
{
    TNKRVIS_CLONEABLE_MACRO(ImageFileRead) 
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
                TnkrVis::IO::ImageFile::Read(inFileName, inAsimage);
                outImageData = Data(ImageData, inAsimage);
            }
            else if(inAsimageGPU != NULL)
            {
                TnkrVis::IO::ImageFile::Read(inFileName, inAsimageGPU);
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
    TNKRVIS_CLONEABLE_MACRO(ImageFileWrite) 
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
                TnkrVis::IO::ImageFile::Write(inFileName, inAsimage);
                outImageData = Data(ImageData, inAsimage);
            }
            else if(inAsimageGPU != NULL)
            {
                TnkrVis::IO::ImageFile::Write(inFileName, inAsimageGPU);
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