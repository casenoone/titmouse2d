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

		//�����ǰ�����뵱ǰ�����ľ��볬����С���룬���ҵ�ǰ���Ӳ��ڵ�ǰ�������
		//��ֱ���øý����Ϊһ��������м���
		if (dis > _minDis && !node->box.IsInBox(p)) {
			auto temp1 = dis * dis + _factor * _factor;
			auto temp2 = std::pow(temp1, 3 / 2.0);
			auto temp3 = 1.0 * node->cmass * dis_v / temp2;
			sum += temp3;
		}

		//���򣬾ͼ����ݹ飬ֱ��������С���룬����������Ҷ�ӽ��
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

	//�ܲ���д�ԣ��Ϳ������ˣ����ɣ�
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

	//�ɳ����ӣ���ֹ����֮�������������µ���ֵ��ը
	double _factor = 0.00001;
};