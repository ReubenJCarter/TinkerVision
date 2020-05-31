#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include <string>

namespace TnkrVis
{
namespace IO
{

class TINKERVISION_EXPORT CameraCapture
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CameraCapture(); 
        ~CameraCapture(); 
        bool Open();
        bool Close();
        bool GetFrame(Image* frameImage);
        bool GetFrame(ImageGPU* frameImage);
        int GetFrameWidth(); 
        int GetFrameHeight(); 
        bool IsOpen(); 
};
	
}
}