#include "BoundingBox.h"

#include <glm/glm.hpp>

namespace Visi
{

BoundingBox::BoundingBox()
{
	rotation = 0;
	position.x = 0;
	position.y = 0;
	extends.x = 0;
	extends.y = 0; 
}

Mat3& BoundingBox::GetTransform()
{
	Mat3 m; 

	return m;
}


}