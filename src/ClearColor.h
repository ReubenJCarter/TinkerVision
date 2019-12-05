#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"
#include <glm/glm.hpp>

namespace Visi
{

class VISI_EXPORT ClearColor
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ClearColor(); 
        ~ClearColor(); 
        void SetColor(glm::vec4 c); 
        void SetColor(glm::vec3 c); 
		void Run(ImageGPU* output); 
        void Run(Image* output); 
};
	
}