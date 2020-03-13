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
        inline void AllocateLike(Image* referanceImage){Allocate(referanceImage->GetWidth(), referanceImage->GetHeight(), referanceImage->GetType());}
        void AllocateLike(ImageGPU* referanceImage);
		void Allocate(unsigned int w, unsigned int h, ImageType t);
        void Deallocate();
        inline unsigned int GetWidth() {return width;}
		inline unsigned int GetHeight() {return height;}
        inline ImageType GetType() {return type;}
        inline unsigned char* GetData() {return data;}
        void Copy(ImageGPU* image); 
        void Copy(Image* image); 
        bool IsSameDimensions(ImageGPU* image); 
        bool IsSameDimensions(Image* image); 
};
	
}