#include "Plane2.h"


Plane2::Plane2() {

}

Plane2::~Plane2() {

}



Plane2::Plane2(const Vector2<double>& _ponit1,
	const Vector2<double>& _point2,
	bool isFliped) :
	point1(_ponit1), point2(_point2) {

	//�������Ĵ���������Ĭ�϶�άƽ��ķ�����Ϊ��
	//point1ָ��point2����������ʱ��ת90��
	auto temp_vec = (point2 - point1).getNormalize();
	normal = Vector2<double>(-temp_vec.y, temp_vec.x);
	if (isFliped == true) {
		normal = -1.0 * normal;
	}

	SurfaceElement2 e(point1, point2, normal);
	_data.push_back(e);
	

}
	

Plane2::Plane2(const Plane2& other) {
	*this = other;
}


Vector2<double> Plane2::closestPoint(const Vector2<double>& otherPoint)const {
	return ExplicitSurface2::closestPoint(otherPoint);
}


double Plane2::closestDistance(const Vector2<double>& otherPoint)const {
	return ExplicitSurface2::closestDistance(otherPoint);
}

Surface2::SurfaceQueryResult Plane2::getClosedInformation(const Vector2<double>& otherPoint) {
	return ExplicitSurface2::getClosedInformation(otherPoint);
}