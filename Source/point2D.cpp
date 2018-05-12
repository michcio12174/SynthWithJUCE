#include "point2D.h"

point2D::point2D():
	x(0),
	y(0)
{
}

point2D::point2D(float x, float y):
	x(x),
	y(y)
{
}

point2D::point2D(point2D &point) :
	x(point.x),
	y(point.y)
{
}

point2D::~point2D()
{
}

float point2D::distance(point2D B)
{
	return sqrt(pow(B.x - this->x, 2) + pow(B.y - this->y, 2));
}
