#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


#include "Vector2.hpp"

class BoundingBox2 {
public:

	BoundingBox2();

	~BoundingBox2();

	BoundingBox2(const Vector2D& point1, const Vector2D& point2);

	bool IsInBox(const Vector2D& p)const;

public:
	Vector2D lowerCorner;

	Vector2D upperCorner;

};

inline bool BoundingBox2::IsInBox(const Vector2D& p)const {
	if (p.x > lowerCorner.x &&
		p.x<upperCorner.x &&
		p.y>lowerCorner.y &&
		p.y < upperCorner.y) {
		return true;
	}
	return false;
}
#endif