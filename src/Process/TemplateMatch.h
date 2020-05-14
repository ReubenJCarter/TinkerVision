#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT TemplateMatch
{
    public:
        enum MatchMode{MATCH_SAD, MATCH_CORR};

    private: 
        class Internal;
        Internal* internal;

	public:
		TemplateMatch(); 
        ~TemplateMatch(); 
        void SetMatchMode(MatchMode mm); 
        void SetNormalized(bool n); 
		void Run(ImageGPU* input, ImageGPU* match, ImageGPU* output); 
        void Run(Image* input, Image* match, Image* output); 
};
	
}
}