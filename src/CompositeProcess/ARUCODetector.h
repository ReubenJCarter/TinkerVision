#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace CompositeProcess
{
	
class VISI_EXPORT ARUCODetector
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ARUCODetector(); 
        ~ARUCODetector(); 
        
		void Run(ImageGPU* input); 
        void AddDictionaryEntry(Image* entry, int id);
};
	
}
}