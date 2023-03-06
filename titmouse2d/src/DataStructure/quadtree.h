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
		//四个子树指针
		std::array<std::unique_ptr<Node>, 4> ch;
		//存放该结点对应的网格索引
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

	//重置网格
	void resetMapGrid() {
		mapGrid.clear();
		mapGrid.resize(_minRes.x);
		for (auto iter = mapGrid.begin(); iter != mapGrid.end(); iter++) {
			iter->resize(_minRes.y);
		}
	}

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

	void buildQuadTree() {
		build(root, 0, _minRes.x, 0, _minRes.y);
	}

	//xl:xleft
	//xr:xright
	//yd:ydown
	//yu:yup

	//网格-结点分布：
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

		//为子结点分配内存
		for (int i = 0; i < 4; ++i) {
			node->ch[i] = std::make_unique<QuadTree::Node>();
		}

		//当xl-xr等于2时，达到最细分辨率
		//确定叶子结点坐标
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
	//树的根节点
	std::unique_ptr<Node> root;

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