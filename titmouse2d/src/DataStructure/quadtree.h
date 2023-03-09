#pragma once
#include <iostream>
#include <vector>
#include <array>
#include "../Vector2.hpp"


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
		//四个子树指针
		std::array<std::unique_ptr<Node>, 4> ch;
		//存放该结点对应的网格索引
		std::unique_ptr<Vector2I> gridIdx = nullptr;

		double cmass = 0.0;

		//质量中心
		Vector2D mcenter;
	};


public:
	BarnesHut() = default;

	BarnesHut(Vector2I minRes_, std::vector<Vector2D>& pos_) :
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
	void buildMapGrid();
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
		std::unique_ptr<BarnesHut::Node>& node,
		int xl, int xr, int yd, int yu);


	//遍历树的叶结点
	void findLeafNode(std::unique_ptr<BarnesHut::Node>& node) {
		if (node == nullptr)return;
		std::cout << "质量：" << node->cmass << "位置:" << node->mcenter.x << "," << node->mcenter.y << std::endl;

		if (node->isLeafNode()) {
			return;
		}
		for (int i = 0; i < 4; ++i)
			findLeafNode(node->ch[i]);
	}

	void test() {

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