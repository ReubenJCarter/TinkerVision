#include "BoundingBox.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>


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

Mat3 BoundingBox::GetTransform()
{
	const static float DEGTORAD = 0.01745329251;
	float sinAng = sin(rotation * DEGTORAD);
	float cosAng = cos(rotation * DEGTORAD);
	Mat3 m;
	m.col[0] = Vec3(cosAng, sinAng, 0); 
	m.col[1] = Vec3(-sinAng, cosAng, 0);
	
	m.col[2] = Vec3(position.x, position.y, 1); 
	return m;
}


}