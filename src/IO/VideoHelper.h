#pragma once

#include "Viso_export.h"

#include "../Core/ImageGPU.h"
#include "../Core/Image.h"

#include <string>
#include <functional> 

namespace Viso
{
namespace IO
{

class VISO_EXPORT VideoHelper
{
    private: 
        class Internal;
        Internal* internal;

	public:
		VideoHelper(); 
        ~VideoHelper(); 
        bool Open(std::string fileSrc);
        int GetFrameWidth();
        int GetFrameHeight(); 
        bool Close(); 
        bool NextFrame(std::function<void(Viso::ImageGPU*, Viso::Image*)> useFrame); 
        bool IsOpen(); 
};
	
}
}