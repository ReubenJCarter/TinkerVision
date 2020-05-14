#pragma once

#include "TinkerVision_export.h"

#include "Image.h"
#include "ImageGPU.h"
#include "Context.h"

#include <string>

namespace TnkrVis
{
	
/*
 *Window
 *
 */
	
class TINKERVISION_EXPORT Window
{        
    private:
        class Internal;
        Internal* internal;

	public:
        Window(int w, int h, Context* shareContext, std::string title=""); 
        ~Window(); 
        void DrawImage(Image* image);
        void DrawImage(ImageGPU* image);
        void Clear(float r, float g, float b, float a);
        void Refresh();          
        bool ShouldClose(); 
};
	
}