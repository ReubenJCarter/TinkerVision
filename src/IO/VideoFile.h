#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include <string>

namespace Visi
{

class VISI_EXPORT VideoFile
{
    private: 
        class Internal;
        Internal* internal;

	public:
		VideoFile(); 
        ~VideoFile(); 
        bool Open(std::string fileSrc);
        int GetStreamCount(); 
        bool Close();
        bool LoadNextFrame(); 
        void SwapBuffers(); 
        bool GetFrame(Image* frameImage, int streamInx=0);
        int GetFrameWidth(int streamInx=0); 
        int GetFrameHeight(int streamInx=0); 
        bool AtEnd(int streamInx=0); 
        bool IsOpen(); 
};
	
}