#ifndef RAY2_H
#define RAY2_H

#include "../Vector2.hpp"

class Ray2 {
public:
	Ray2();

	~Ray2();

	Ray2(Vector2D origin, Vector2D orientation);

	Vector2D origin();

	Vector2D orientation();

private:

	Vector2D _origin;

	Vector2D _orientation;
};


#endif