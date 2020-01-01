#pragma once

#include "Visi_export.h"

#include "VectorMath.h"

namespace Visi
{
	
class VISI_EXPORT BoundingBox
{
        public:
				float rotation; 
				Vec2 position; 
				Vec2 extends;
				
                BoundingBox();
				Mat3 GetTransform(); 
};
	
}