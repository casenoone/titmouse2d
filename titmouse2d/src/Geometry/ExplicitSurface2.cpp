#include "ExplicitSurface2.h"


SurfaceElement2::SurfaceElement2() {

}

SurfaceElement2::~SurfaceElement2() {

}

SurfaceElement2::SurfaceElement2(Vector2<double> _start, Vector2<double> _end, Vector2<double> _normal):
start(_start),end(_end),normal(_normal)
{
	normal.normalize();
}

ExplicitSurface2::ExplicitSurface2() {

}

ExplicitSurface2::~ExplicitSurface2() {

}

ExplicitSurface2::ExplicitSurface2(const vector<SurfaceElement2>& data):
	_data(data)
{}


Vector2<double> ExplicitSurface2::closestPoint(const Vector2<double>& otherPoint)const {

}

double ExplicitSurface2::closestDistance(const Vector2<double>& otherPoint)const {

}