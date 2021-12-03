#include "Ray2.h"

Ray2::Ray2():
_origin(Vector2<double>()),_orientation(Vector2<double>())
{

}

Ray2::~Ray2() {

}

Ray2::Ray2(Vector2<double> origin, Vector2<double> orientation):
	_origin(origin), _orientation(orientation)

{

}

Vector2<double> Ray2::origin() {
	return _origin;
}

Vector2<double> Ray2::orientation() {
	return _orientation;
}