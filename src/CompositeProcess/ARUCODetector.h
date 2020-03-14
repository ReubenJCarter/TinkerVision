#pragma once

#include "Visi_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include <vector>

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
        
		void Run(ImageGPU* input, std::vector<Visi::Contour>* markerQuads, std::vector<int>* markerIds, bool retAllQuads=false); 
        void AddDictionaryEntry(Image* entry, int id);
        void AddDictionaryEntry(std::vector<bool>* bitsequence, int size, int id);
        void SetMaxBitError(int mbe); 
};
	
}
}