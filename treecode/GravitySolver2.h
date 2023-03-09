#pragma once
#include <iostream>
#include "../titmouse2d/src/Vector2.hpp"
#include "BarnesHut.h"

class GravitySolver2 {
public:
	GravitySolver2() = default;

	GravitySolver2(std::vector<Vector2D>& pos_, Vector2I minRes_) :
		pos(pos_), _minRes(minRes_) {}

	void rebuild() {
		_barnesHut = std::make_shared<BarnesHut>(pos, _minRes);
	}

	void solveGravity() {

	}

	void timeIntegration(double dt) {

	}

	void onAdvanceTimeStep(double dt) {

	}

	void toZero() {
		auto size = pos.size();
		_force.clear();
		_force.resize(size);
	}

public:
	std::vector<Vector2D>& pos;
private:
	Vector2I _minRes;
	BarnesHutPtr _barnesHut;
	double _minDis = 0.3;
	std::vector<Vector2D> _vel;
	std::vector<Vector2D> _force;
};