#pragma once

#include "Visi_export.h"

#include "ImageType.h"

namespace Visi
{
	
class ImageGPU; 

/*
 *Image
 *
 */
	
class VISI_EXPORT Image
{		
	private:
        ImageType type; 
		unsigned int width;
		unsigned int height;
        unsigned char* data; 

	public:
		Image(); 
        ~Image(); 
		void Allocate(unsigned int w, unsigned int h, ImageType t);
        void Deallocate();
        unsigned int GetWidth();
		unsigned int GetHeight();
        ImageType GetType();
        unsigned char* GetData();
        void Copy(ImageGPU* image); 
        bool IsSameDimensions(ImageGPU* image); 
        bool IsSameDimensions(Image* image); 
};
	
}