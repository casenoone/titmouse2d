#ifndef SURFACE2_H
#define SURFACE2_H

#include <iostream>
using namespace std;

#include "../Vector2.hpp"

//������໮��Ϊ��ʽ�������ʽ����
//Ȼ����������ʽ����ʽ֮���ת��
class Surface2 {
public:
	Surface2();

	virtual ~Surface2();

	virtual Vector2<double> closestPoint(const Vector2<double>& otherPoint)const = 0;

	virtual double closestDistance(const Vector2<double>& otherPoint)const = 0;



public:
	struct SurfaceQueryResult final {
		double distance;
		Vector2<double> point;
		Vector2<double> normal;
		Vector2<double> velocity;
	};


	SurfaceQueryResult _surfaceQueryResult;

	virtual SurfaceQueryResult getClosedInformation(const Vector2<double>& otherPoint) = 0;

};

using Surface2Ptr = shared_ptr<Surface2>;


#endif