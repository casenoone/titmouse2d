#include "ExplicitSurface2.h"
#include "../ConstVar.h"

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
	
	
	return Vector2<double>();
}

double ExplicitSurface2::closestDistance(const Vector2<double>& otherPoint)const {
	return 0.0;
}


Surface2::SurfaceQueryResult ExplicitSurface2::getClosedInformation(const Vector2<double>& otherPoint) {

	auto minDis_2 = kMaxD;
	auto minPoint = Vector2<double>();
	auto minNormal = Vector2<double>();
	for (auto i = _data.begin(); i != _data.end(); ++i) {
		auto A = i->start;
		auto B = i->end;
		auto N = i->normal;

		auto BA = A - B;
		auto BO = otherPoint - B;

		double r = BO.dot(BA) / BA.getLength();
		double dis_2 = BO.getLengthSquared() - r * r;

		auto BA_normalized = BA.getNormalize();

		//求投影点
		auto P = B + r * BA_normalized;

		auto PA = A - P;
		auto PB = B - P;
		
		//如果投影点在线段内
		if (PA.dot(PB) < 0) {
			auto temp_dis2 = (otherPoint - P).getLengthSquared();
			if (temp_dis2 < minDis_2) { minDis_2 = temp_dis2; minPoint = P;minNormal = N;}
		}
		//如果投影点不在线段内，那么就判断其离哪个端点最近
		else {

			auto PA_dis2 = PA.getLengthSquared();
			auto PB_dis2 = PB.getLengthSquared();
			
			if (PA_dis2 <= PB_dis2) {
				auto temp_dis2 = (otherPoint - A).getLengthSquared();
				if (temp_dis2 < minDis_2) { minDis_2 = temp_dis2; minPoint = A; minNormal = N;}
			}
			else {
				auto temp_dis2 = (otherPoint - B).getLengthSquared();
				if (temp_dis2 < minDis_2) { minDis_2 = temp_dis2; minPoint = B; minNormal = N;}
			}
		}
		
		
	}

	_surfaceQueryResult.distance = sqrt(minDis_2);
	_surfaceQueryResult.normal = minNormal;
	_surfaceQueryResult.point = minPoint;
	_surfaceQueryResult.velocity = Vector2<double>();

	return _surfaceQueryResult;

}