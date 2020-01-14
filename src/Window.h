#pragma once

#include "Visi_export.h"

#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
/*
 *Context
 *
 */
	
class VISI_EXPORT Window
{        
    private:
        class Internal;
        Internal* internal;

	public:
        Window(int w, int h); 
        ~Window(); 
        void DrawImage(Image* image);
        void DrawImage(ImageGPU* image);
        void Clear(float r, float g, float b, float a);
        void Refresh();          
        bool ShouldClose(); 
};
	
}