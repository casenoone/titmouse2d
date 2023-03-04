#pragma once
#include "Vector2.hpp"
class BoundingBox2 {
public:
	BoundingBox2() = default;

	BoundingBox2(const Vector2D& lower_, const Vector2D& upper_) :
		lower(lower_), upper(upper_) {
	}

public:
	Vector2D lower;
	Vector2D upper;
};

