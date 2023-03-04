#pragma once
#include "Vector2.hpp"
#include "Sphere2.h"
#include "BoundingBox2.h"
class Ray2 {
public:
	Ray2() = default;
	Ray2(const Vector2D& origin_, const Vector2D& direction_) :
		origin(origin_), direction(direction_.getNormalize()) {};

	bool IsInTersect(const BoundingBox2& box_) const {
		double x_min = 0.0;
		double y_min = 0.0;
		double x_max = 0.0;
		double y_max = 0.0;

		double dir_x = direction.x;
		double dir_y = direction.y;
		double ox = origin.x;
		double oy = origin.y;

		//如果光线沿y轴且起点不在包围盒内
		//则不会与包围盒相交
		if (abs(dir_x) < 0.0001 && (ox < box_.lower.x || ox>box_.upper.x)) {
			return false;
		}
		else {
			if (dir_x > 0) {
				x_min = (box_.lower.x - ox) / dir_x;
				x_max = (box_.upper.x - ox) / dir_x;

			}
			else {
				x_min = (box_.upper.x - ox) / dir_x;
				x_max = (box_.lower.x - ox) / dir_x;

			}
		}

		//如果光线沿x轴且起点不在包围盒内
		//则不会与包围盒相交
		if (abs(dir_y) < 0.0001 && (oy < box_.lower.y || oy>box_.upper.y)) {
			return false;
		}
		else {
			if (dir_y >= 0)
			{
				y_min = (box_.lower.y - oy) / dir_y;
				y_max = (box_.upper.y - oy) / dir_y;
			}
			else
			{
				y_min = (box_.upper.y - oy) / dir_y;
				y_max = (box_.lower.y - oy) / dir_y;
			}

		}

		auto t0 = std::max(x_min, y_min);
		auto t1 = std::min(x_max, y_max);
		return t0 < t1;
	}

	//方向需要归一化


public:
	Vector2D origin;
	Vector2D direction;
};