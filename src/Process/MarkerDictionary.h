#pragma once

#include "Viso_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Viso
{
namespace Process
{
	
class VISO_EXPORT MarkerDictionary
{
    private: 
        class Internal;
        Internal* internal;

	public:
		MarkerDictionary(); 
        ~MarkerDictionary(); 
        void AddEntry(Image* im, int id); 
        int Lookup(Image* input); 
        void SetMaxBitError(int mbe); 
};
	
}
}