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

		if (node->isLeafNode()) {

			auto& idx = _barnesHut->root->gridIdx;
			auto& list = _barnesHut->mapGrid[idx->x][idx->y];

			for (int i = 0; i < list.size(); ++i) {
				auto i_pos = pos[list[i]];
				double pdis = p.dis(i_pos);
				auto temp1 = pdis * pdis + _factor * _factor;
				auto temp2 = std::pow(temp1, 3 / 2.0);
				auto temp3 = 1.0 * 1.0 * dis_v / temp2;
				sum += temp3;
			}
			return;
		}

		//如果当前粒子与当前树结点的距离超过最小距离，并且当前粒子不在当前树结点中
		//则直接用该结点作为一个整体进行计算
		if (dis > _minDis && !node->box.IsInBox(p)) {
			auto temp1 = dis * dis + _factor * _factor;
			auto temp2 = std::pow(temp1, 3 / 2.0);
			auto temp3 = 1.0 * node->cmass * dis_v / temp2;
			sum += temp3;
		}

		//否则，就继续递归，直到超过最小距离，或者是遇到叶子结点
		else {
			for (int i = 0; i < 4; ++i) {
				traverse(node->ch[i], p, sum);
			}
		}
	}

	void solveGravity() {
		int num = pos.size();
		for (int i = 0; i < num; ++i) {
			traverse(_barnesHut->root, pos[i], _force[i]);
		}
	}

	//能不能写对，就看这里了！开干！
	void timeIntegration(double dt) {

	}

	void onAdvanceTimeStep(double dt) {
		toZero();
		solveGravity();
		timeIntegration(dt);
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