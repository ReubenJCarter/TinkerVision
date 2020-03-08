#pragma once

#include "Visi_export.h"

#include "VectorMath.h"

namespace Visi
{
	
/**
 *A 2D Bounding Box. The bounding box fits via a rotation position and extends. 
 *The 3x3 Transform can be aquired using the GetTransform function.
 */
class VISI_EXPORT BoundingBox
{
	public:
		/**The rotation of the bounding box in degrees.*/ 
		float rotation; 
		
		/**The position of the bounding box.*/
		Vec2 position; 
		 
		/**This is the ammount the bounding box extends along each axis, such that the size=2*extends.*/
		Vec2 extends;
		
		/**The constructor. Position rotation and extends de3fault to zero.*/
		BoundingBox();

		/**Returns the affine transform of the bouding box, 
		 * T = (cos(rot), sin(rot), 0, -sin(rot), cos(rot),0, 0, 0, 0) * (0,0,p.x, 0,0,p.y, 0,0,1) .
		 * The transform does not include the extends.
		 */
		Mat3 GetTransform(); 
};
	
}