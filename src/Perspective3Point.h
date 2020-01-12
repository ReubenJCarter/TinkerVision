#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{

class VISI_EXPORT Perspective3Point
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Perspective3Point(); 
        ~Perspective3Point(); 
};
	
}