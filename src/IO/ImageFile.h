#pragma once

#include <string>

#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

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
}