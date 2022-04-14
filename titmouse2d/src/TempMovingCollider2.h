#ifndef TEMPMOVINGCOLLIDER2_H
#define TEMPMOVINGCOLLIDER2_H

#include <iostream>
#include <cmath>

#include "Array.hpp"
#include "Vector2.hpp"
#include "random.h"

//临时弄一个移动边界，有空再去修改collider类，使其具有移动属性

inline double resolveMovingCollision(Array<Vector2D> postions,
	Array<Vector2D> velocities) {

	double min = 0;
	static double angle = min;
	double max = 3.1415926535;
	double mid = max / 2;
	double pos_x = 0.0;
	double velocity = 0.1;

	pos_x = 2 - 0.5 * fabs(sin(angle));

	angle += velocity;
	if (angle >= max)
		angle = min;

	double epllsion = 1e-5;

	int numberOfParticles = postions.dataSize();
	for (int i = 0; i < numberOfParticles; ++i) {
		if (postions[i].x >= pos_x) {
			postions[i].x = pos_x;
			if (angle > mid) {
				postions[i].x -= epllsion * random_double(0, 1);
				velocities[i].x = -velocities[i].x;
			}
			else {
				velocities[i].x = velocities[i].x;
			}
		}
	}

	return pos_x;

}



#endif