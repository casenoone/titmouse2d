#include "Ray2.h"

Ray2::Ray2() :
	_origin(Vector2D()), _orientation(Vector2D())
{

}

Ray2::~Ray2() {

}

Ray2::Ray2(Vector2D origin, Vector2D orientation) :
	_origin(origin), _orientation(orientation)

{

}

Vector2D Ray2::origin() {
	return _origin;
}

Vector2D Ray2::orientation() {
	return _orientation;
}