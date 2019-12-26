#pragma once

#include "Visi_export.h"

#include "ImageType.h"
#include "Image.h"
#include "ImageGPU.h"
#include "Contour.h"

#include "VectorMath.h"

namespace Visi
{

class VISI_EXPORT Renderer
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Renderer(); 
        ~Renderer(); 
        void Clear();
        void Run(Image* input, Image* output); 
        void AddPolyLine(std::vector<Vec2>* pl, Color color=Color(1.0f, 0.0f, 0.0f, 1.0f), float lineWidth=1, bool closed=false); 
        void AddCircle(Vec2 centre, float radius, Color color=Color(1.0f, 0.0f, 0.0f, 1.0f), bool filled=false, float borderWidth=1); 
        void AddContours(std::vector<Contour>* contours, bool renderVerts=true, float vertRad=2, bool renderLines=true, bool closed=true); 
};
	
}