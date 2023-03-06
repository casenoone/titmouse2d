#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "../Vector2.hpp"


class QuadTree {
public:
	class Node {
	public:
		Node() = default;
		~Node() {}
	public:
		//�ĸ�����ָ��
		std::array<std::unique_ptr<Node>, 4> ch;
		//��Ÿý���Ӧ����������
		std::unique_ptr<Vector2I> gridIdx = nullptr;
	};


public:
	QuadTree() = default;

	QuadTree(Vector2I minRes_, std::vector<Vector2D>& pos_) :
		pos(pos_), _minRes(minRes_) {
		root = std::make_unique<Node>();
		resetMapGrid();
		buildMapGrid();
		buildQuadTree();
	}

	//��������
	void resetMapGrid() {
		mapGrid.clear();
		mapGrid.resize(_minRes.x);
		for (auto iter = mapGrid.begin(); iter != mapGrid.end(); iter++) {
			iter->resize(_minRes.y);
		}
	}

	//������ӳ�䵽�Ĳ�����ϸ�ֱ���������
	void buildMapGrid() {
		int particleIdx = 0;

		for (int i = 0; i < pos.size(); ++i) {
			double maxWidth = 2;
			double maxHeight = 2;
			double tempX = pos[i].x;
			double tempY = pos[i].y;

			double scaleX = maxWidth / _minRes.x;
			double scaleY = maxHeight / _minRes.y;

			int gridX = floor(tempX / scaleX);
			int gridY = floor(tempY / scaleY);
			int k = 1;

			if (gridX >= _minRes.x)
				gridX = _minRes.x - k;
			if (gridX < 0)
				gridX = 0;
			if (gridY >= _minRes.y)
				gridY = _minRes.y - k;
			if (gridY < 0)
				gridY = 0;

			mapGrid[gridX][gridY].push_back(i);
		}
	}

	void buildQuadTree() {
		build(root, 0, _minRes.x, 0, _minRes.y);
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
	void build(
		std::unique_ptr<QuadTree::Node>& node,
		int xl, int xr, int yd, int yu) {
		if ((xr - xl) < 2) {
			return;
		}

		//Ϊ�ӽ������ڴ�
		for (int i = 0; i < 4; ++i) {
			node->ch[i] = std::make_unique<QuadTree::Node>();
		}

		//��xl-xr����2ʱ���ﵽ��ϸ�ֱ���
		//ȷ��Ҷ�ӽ������
		if ((xl - xr) == 2) {
			node->ch[0]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl, yd));
			node->ch[1]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl + 1, yd));
			node->ch[2]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl, yd + 1));
			node->ch[3]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl + 1, yd + 1));
		}

		build(node->ch[0], xl, (xl + xr) * 0.5, yd, (yd + yu) * 0.5);
		build(node->ch[1], (xl + xr) * 0.5, xr, yd, (yd + yu) * 0.5);
		build(node->ch[2], xl, (xl + xr) * 0.5, (yd + yu) * 0.5, yu);
		build(node->ch[3], (xl + xr) * 0.5, xr, (yd + yu) * 0.5, yu);
	}

public:
	//���ĸ��ڵ�
	std::unique_ptr<Node> root;

	//�����ܲ���
	int totalLevel = 1;

	//����λ�õ�����
	std::vector<Vector2D>& pos;

	//�������ڵ�ǰ�������������ӵı��
	std::vector<std::vector<std::vector<int>>> mapGrid;

private:
	//��ϸ�ֱ��ʣ�����Ϊ2����������
	Vector2I _minRes;
};