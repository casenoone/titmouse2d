#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/boundingbox2.h"

class BarnesHut {
public:
	class Node {
	public:
		Node() = default;
		~Node() {}
		bool isLeafNode() {
			if (ch[0] == nullptr &&
				ch[1] == nullptr &&
				ch[2] == nullptr &&
				ch[3] == nullptr) {
				return true;
			}
			return false;
		}
	public:
		//�ĸ�����ָ��
		std::array<std::unique_ptr<Node>, 4> ch;
		//��Ÿý���Ӧ����������
		std::unique_ptr<Vector2I> gridIdx = nullptr;

		double cmass = 0.0;

		//��������
		Vector2D mcenter;

		double Q;
		Vector2D Q_k;

		BoundingBox2 box;
	};

private:
	//��������
	void resetMapGrid() {
		mapGrid.clear();
		mapGrid.resize(_minRes.x);
		for (auto iter = mapGrid.begin(); iter != mapGrid.end(); iter++) {
			iter->resize(_minRes.y);
		}
	}

	//������ӳ�䵽�Ĳ�����ϸ�ֱ���������
	void buildMapGrid();
	void buildQuadTree() {
		build(root, 0, _minRes.x, 0, _minRes.y);
	}
	void build(
		std::unique_ptr<BarnesHut::Node>& node,
		int xl, int xr, int yd, int yu);
	//��������Ҷ���
	void findLeafNode(std::unique_ptr<BarnesHut::Node>& node) {
		if (node == nullptr)return;

		if (node->isLeafNode()) {
			return;
		}
		for (int i = 0; i < 4; ++i)
			findLeafNode(node->ch[i]);
	}

	void buildBoundingBox(
		std::unique_ptr<BarnesHut::Node>& node,
		int xl, int xr, int yd, int yu) {

		double scale = _minRes.x / 2.0;
		double low_x = xl / scale;
		double up_x = xr / scale;
		double low_y = yd / scale;
		double up_y = yu / scale;
		node->box = BoundingBox2(Vector2D(low_x, low_y), Vector2D(up_x, up_y));
	}


public:
	BarnesHut() = default;

	BarnesHut(Vector2I minRes_, std::vector<Vector2D>& pos_, std::vector<Vector2D>& gamma_) :
		pos(pos_), _minRes(minRes_), gamma(gamma_) {
		root = std::make_unique<Node>();
		resetMapGrid();
		buildMapGrid();
		buildQuadTree();
	}

	std::unique_ptr<BarnesHut::Node> rebuildTree() {

	}

	//��������Ҷ���
	void findLeafNode() {
		findLeafNode(root);
	}

	//xl:xleft
	//xr:xright
	//yd:ydown
	//yu:yup

	//����-���ֲ���
	//2*****3
	//*******
	//*******
	//0*****1

public:
	//���ĸ��ڵ�
	std::unique_ptr<Node> root;
public:
	//��ϸ�ֱ��ʣ�����Ϊ2����������
	Vector2I _minRes;
	//����λ�õ�����
	std::vector<Vector2D>& pos;
	std::vector<Vector2D>& gamma;

	//�������ڵ�ǰ�������������ӵı��
	std::vector<std::vector<std::vector<int>>> mapGrid;

	double mass = 0.03;

};

using BarnesHutPtr = std::shared_ptr<BarnesHut>;
