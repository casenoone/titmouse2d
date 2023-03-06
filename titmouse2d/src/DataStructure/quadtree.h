#pragma once
#include <iostream>
#include <vector>
#include "../Vector2.hpp"


class QuadTree {
public:
	class Node {
	public:
		Node() = default;
		~Node() {}
	public:
		//�ĸ�����ָ��
		std::unique_ptr<Node> p1 = nullptr;
		std::unique_ptr<Node> p2 = nullptr;
		std::unique_ptr<Node> p3 = nullptr;
		std::unique_ptr<Node> p4 = nullptr;

		//��Ÿý���Ӧ����������
		std::unique_ptr<Vector2I> gridIdx = nullptr;

		//��ǵ�ǰ�ڵ������ĵڼ���
		int level = 0;
	};


public:
	QuadTree() = default;

	QuadTree(Vector2I minRes_, std::vector<Vector2D>& pos_) :
		pos(pos_), _minRes(minRes_) {}

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

	//��������
	void resetMapGrid() {
		mapGrid.clear();
		mapGrid.resize(_minRes.x);
		for (auto iter = mapGrid.begin(); iter != mapGrid.end(); iter++) {
			iter->resize(_minRes.y);
		}
	}

public:
	//���ĸ��ڵ�
	std::shared_ptr<Node> root;

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