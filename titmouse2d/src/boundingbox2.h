#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


#include "Vector2.hpp"

class BoundingBox2 {
public:

	BoundingBox2();

	~BoundingBox2();

	BoundingBox2(const Vector2D& point1, const Vector2D& point2);


public:
	Vector2D lowerCorner;

	Vector2D upperCorner;

};


#endif