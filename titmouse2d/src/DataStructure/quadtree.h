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
		//四个子树指针
		std::unique_ptr<Node> p1 = nullptr;
		std::unique_ptr<Node> p2 = nullptr;
		std::unique_ptr<Node> p3 = nullptr;
		std::unique_ptr<Node> p4 = nullptr;

		//存放该结点对应的网格索引
		std::unique_ptr<Vector2I> gridIdx = nullptr;

		//标记当前节点在树的第几层
		int level = 0;
	};


public:
	QuadTree() = default;

	QuadTree(Vector2I minRes_, std::vector<Vector2D>& pos_) :
		pos(pos_), _minRes(minRes_) {}

	//将粒子映射到四叉树最细分辨率网格中
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

	//重置网格
	void resetMapGrid() {
		mapGrid.clear();
		mapGrid.resize(_minRes.x);
		for (auto iter = mapGrid.begin(); iter != mapGrid.end(); iter++) {
			iter->resize(_minRes.y);
		}
	}

public:
	//树的根节点
	std::shared_ptr<Node> root;

	//树的总层数
	int totalLevel = 1;

	//粒子位置的引用
	std::vector<Vector2D>& pos;

	//保存属于当前格子里所有粒子的编号
	std::vector<std::vector<std::vector<int>>> mapGrid;

private:
	//最细分辨率，必须为2的整数次幂
	Vector2I _minRes;
};