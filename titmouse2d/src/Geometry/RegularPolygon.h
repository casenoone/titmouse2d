#ifndef REGULARPOLYGON_H
#define REGULARPOLYGON_H

#include "ExplicitSurface2.h"
#include "../Vector2.hpp"

class RegularPolygon : public ExplicitSurface2 {
public:
	RegularPolygon(int edgeNum_, Vector2D center_, double r_);

	Vector2D center()const {
		return _center;
	}

	double r()const {
		return _r;
	}

private:
	int _edgeNum = 7;
	Vector2D _center;
	double _r = 0.3;
};

using RegularPolygonPtr = shared_ptr<RegularPolygon>;


#endif