#pragma once
#include "Vector2.hpp"
#include "BoundingBox2.h"
#include "Ray2.h"

class Ray2;
class Geometry2 {
public:
	virtual ~Geometry2() {}

	//重心
	const Vector2D& barycenter() {
		return _barycenter;
	}
	//判断物体与射线是否相交
	virtual bool IsInTersect(const Ray2& ray)const = 0;

public:
	BoundingBox2 box;
	size_t number = 0;
protected:
	Vector2D _barycenter;
};

typedef std::shared_ptr<Geometry2> Geometry2Ptr;
