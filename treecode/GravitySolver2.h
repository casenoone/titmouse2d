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

	void traverse(std::unique_ptr<BarnesHut::Node>& node, const Vector2D& p, Vector2D& sum) {
		if (!node)return;
		auto dis = node->mcenter.dis(p);
		auto dis_v = node->mcenter - p;
		if (dis > _minDis /*并且当前粒子不在当前node内*/) {
			auto temp1 = dis * dis + _factor * _factor;
			auto temp2 = std::pow(temp1, 3 / 2.0);
			auto temp3 = 1.0 * node->cmass * dis_v / temp2;
			sum += temp3;
		}
		else {
			for (int i = 0; i < 4; ++i) {
				traverse(node->ch[i], p, sum);
			}
		}
	}

	void solveGravity() {
		int num = pos.size();
		for (int i = 0; i < num; ++i) {


		}
	}

	void timeIntegration(double dt) {

	}

	void onAdvanceTimeStep(double dt) {
		toZero();
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

	//松弛因子，防止粒子之间距离过近而导致的数值爆炸
	double _factor = 0.00001;
};