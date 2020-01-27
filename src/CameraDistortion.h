#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"

namespace Visi
{
	
class VISI_EXPORT CameraDistortion
{
    private: 
        class Internal;
        Internal* internal;

	public:
		CameraDistortion(); 
        ~CameraDistortion(); 
        void SetRadialCoefs(float k0, float k1, float k2);
        void SetTangentialCoefs(float p0, float p1);
        void SetFocalLength(float fl);
		void Run(ImageGPU* input, ImageGPU* output); 
        void Run(Image* input, Image* output); 
};
	
}