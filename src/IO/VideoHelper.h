#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageGPU.h"
#include "../Core/Image.h"

#include <string>
#include <functional> 

namespace TnkrVis
{
namespace IO
{

class TINKERVISION_EXPORT VideoHelper
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
        bool NextFrame(std::function<void(TnkrVis::ImageGPU*, TnkrVis::Image*)> useFrame); 
        bool IsOpen(); 
};
	
}
}