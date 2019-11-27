#pragma once

#include "Visi_export.h"

#include "ImageType.h"

namespace Visi
{
	
class Image;

/*
 *Image
 *
 */

class VISI_EXPORT ImageGPU
{		
	private:
        ImageType type; 
		unsigned int width;
		unsigned int height;
        unsigned int texture; 

	public:
		ImageGPU(); 
        ~ImageGPU(); 
		void Allocate(unsigned int w, unsigned int h, ImageType t);
        void Deallocate();
        unsigned int GetWidth();
		unsigned int GetHeight();
        ImageType GetType();
        unsigned int GetTexture();
        void Copy(void* data, int w, int h, int offX=0, int offY=0);
        void Copy(Image* image); 
};
	
}