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
        inline void AllocateLike(ImageGPU* referanceImage){Allocate(referanceImage->GetWidth(), referanceImage->GetHeight(), referanceImage->GetType());}
        void AllocateLike(Image* referanceImage);
		void Allocate(unsigned int w, unsigned int h, ImageType t);
        void Deallocate();
        inline int GetWidth() {return (int)width;}
		inline int GetHeight() {return (int)height;}
        inline ImageType GetType() {return type;}
        inline unsigned int GetTexture() {return texture;}
        void Copy(void* data, int w, int h, int offX=0, int offY=0);
        void Copy(Image* image); 
        bool IsSameDimensions(ImageGPU* image); 
        bool IsSameDimensions(Image* image); 
        bool IsSameSize(ImageGPU* image); 
        bool IsSameSize(Image* image); 
        bool IsSameType(ImageGPU* image); 
        bool IsSameType(Image* image); 
        int GetChannelCount(); 
};
	
}