#pragma once

#include <string>

#include "../Core/Image.h"

namespace Visi
{
namespace IO
{

class VISI_EXPORT ImageFile
{	
    public:
        static bool Write(std::string fileName, Image* image);
        static bool Read(std::string fileName, Image* image); 
};
	
}
}