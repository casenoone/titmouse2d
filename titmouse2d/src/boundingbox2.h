#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


#include "Vector2.hpp"

class BoundingBox2 {
public:

	BoundingBox2();
	
	~BoundingBox2();

	BoundingBox2(const Vector2<double>& point1, const Vector2<double>& point2);


public:
	Vector2<double> lowerCorner;
	
	Vector2<double> upperCorner;

};


#endif