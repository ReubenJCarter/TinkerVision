#pragma once

#include "TinkerVision_export.h"

#include "ImageType.h"

namespace TnkrVis
{
	
class ImageGPU; 

/*
 *Image
 *
 */
	
class TINKERVISION_EXPORT Image
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
        inline int GetWidth() {return (int)width;}
		inline int GetHeight() {return (int)height;}
        inline ImageType GetType() {return type;}
        inline unsigned char* GetData() {return data;}
        void Copy(ImageGPU* image); 
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