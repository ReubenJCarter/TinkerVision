#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{
namespace Process
{
	
class VISI_EXPORT MarkerDictionary
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MarkerDictionary(); 
        ~MarkerDictionary(); 
        void AddEntry(Image* im, int id); 
        int Lookup(Image* input); 
};
	
}
}