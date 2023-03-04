#pragma once
#include "Vector2.hpp"
#include "BoundingBox2.h"
#include "Geometry2.h"


class  Sphere2 :public Geometry2 {
public:
	Sphere2() = default;
	Sphere2(const Vector2D& center_, double r_) :
		center(center_), r(r_)
	{
		box = BoundingBox2(Vector2D(center_.x - r_, center_.y - r_),
			Vector2D(center_.x + r_, center_.y + r_));
		number = counter++;
		//ÉèÖÃÖØÐÄ
		_barycenter = center_;
	}

	bool IsInTersect(const Ray2& ray)const override {
		auto v = ray.origin - center;
		auto dv = ray.direction.dot(v);
		auto v2 = v.getLengthSquared();
		auto r2 = r * r;
		auto term1 = dv * dv;
		auto term2 = v2 - r2;
		if ((term1 - term2) >= 0) {
			return true;
		}
		return false;
	}

public:
	Vector2D center;
	double r = 1.0;

private:
	static size_t counter;
};

typedef std::shared_ptr<Sphere2> Sphere2Ptr;

size_t Sphere2::counter = 0;