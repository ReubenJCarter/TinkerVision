#pragma once

#include "Visi_export.h"

#include "ImageGPU.h"
#include "Image.h"

#include <string>
#include <functional> 

namespace Visi
{

class VISI_EXPORT VideoHelper
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
        bool NextFrame(std::function<void(Visi::ImageGPU*, Visi::Image*)> useFrame); 
        bool IsOpen(); 
};
	
}