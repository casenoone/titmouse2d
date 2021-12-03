#ifndef RAY2_H
#define RAY2_H

#include "../Vector2.hpp"

class Ray2 {
public:
	Ray2();

	~Ray2();

	Ray2(Vector2<double> origin, Vector2<double> orientation);

	Vector2<double> origin();

	Vector2<double> orientation();

private:

	Vector2<double> _origin;

	Vector2<double> _orientation;
};


#endif