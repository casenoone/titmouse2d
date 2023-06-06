#pragma once
#include <iostream>
#include "../titmouse2d/src/Vector2.hpp"
#include "solver.h"

class GravitySolver2 {
public:
	GravitySolver2() = default;

	GravitySolver2(Vector2I minRes_, std::vector<Vector2D>& pos_) :
		pos(pos_), _minRes(minRes_) {
		_vel.resize(pos.size());
	}

	void rebuild() {
		_barnesHut = std::make_shared<BarnesHut>(_minRes, pos);
	}

	void traverse(std::unique_ptr<BarnesHut::Node>& node, const Vector2D& p, Vector2D& sum, Vector2D gamm) {
		if (!node)return;

		auto dis = node->mcenter.dis(p);
		auto dis_v = node->mcenter - p;

		if (node->isLeafNode()) {
			auto& idx = node->gridIdx;
			if (!idx)return;
			auto& list = _barnesHut->mapGrid[idx->x][idx->y];
			for (int i = 0; i < list.size(); ++i) {
				auto i_pos = pos[list[i]];

				sum += node->Q * node->Q_k;
			}
			return;
		}
		//如果当前粒子与当前树结点的距离超过最小距离，并且当前粒子不在当前树结点中
		//则直接用该结点作为一个整体进行计算
		if (dis > _minDis && !node->box.IsInBox(p)) {

			auto temp1 = (1 - std::pow(2.8, -dis * dis) / 0.0001) * (node->mcenter - p).cross(gamm) * (node->mcenter - p).normalize();
			auto temp2 = 2 * 3.1415926 * (node->mcenter - p).getLengthSquared();

			sum += temp1 / temp2;
		}
		else
			for (int i = 0; i < 4; ++i) {
				traverse(node->ch[i], p, sum, gamm);
			}

	}

	void solveGravity() {
		int num = pos.size();
		for (int i = 0; i < num; ++i) {
			traverse(_barnesHut->root, pos[i], _force[i], Vector2D());
			_force[i] *= (9.8 * _barnesHut->mass);
		}
	}

	void timeIntegration(double dt) {
		for (int i = 0; i < pos.size(); ++i) {
			_vel[i] += _force[i] * dt;
			pos[i] += _vel[i] * dt;
		}
	}

	void onAdvanceTimeStep(double dt) {
		rebuild();
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
	double _minDis = 0.1;
	std::vector<Vector2D> _vel;
	std::vector<Vector2D> _force;

	//松弛因子，防止粒子之间距离过近而导致的数值爆炸
	double _factor = 0.01;

};

using GravitySolver2Ptr = std::shared_ptr<GravitySolver2>;
