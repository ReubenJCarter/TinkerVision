#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"
#include "../Core/Contour.h"

#include "../Core/VectorMath.h"

namespace TnkrVis
{
namespace Process
{

class TINKERVISION_EXPORT Renderer
{
    private: 
        class Internal;
        Internal* internal;

	public:
		Renderer(); 
        ~Renderer(); 
        void Clear();
        void Run(Image* input, Image* output); 
        void Run(Image* dst); 
        void AddPolyLine(std::vector<Vec2>* pl, Color color=Color(1.0f, 0.0f, 0.0f, 1.0f), float lineWidth=1, bool closed=false); 
        void AddCircle(Vec2 centre, float radius, Color color=Color(1.0f, 0.0f, 0.0f, 1.0f), bool filled=false, float borderWidth=1); 
        void AddContour(Contour* contour, Color color=Color(1.0f, 0.0f, 0.0f, 1.0f), bool renderVerts=true, float vertRad=2, bool renderLines=true, bool closed=true); 
        void AddContours(std::vector<Contour>* contours, bool renderVerts=true, float vertRad=2, bool renderLines=true, bool closed=true); 
        void AddBoundingBox(BoundingBox* boundingBox, Color color); 
        void Add3DAxis(float fovy, Vec3 pos);
};
	
}
}