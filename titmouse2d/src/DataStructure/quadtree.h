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
		//�ĸ�����ָ��
		std::array<std::unique_ptr<Node>, 4> ch;
		//��Ÿý���Ӧ����������
		std::unique_ptr<Vector2I> gridIdx = nullptr;

		double cmass = 0.0;

		//��������
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
		std::unique_ptr<BarnesHut::Node>& node,
		int xl, int xr, int yd, int yu);


	//��������Ҷ���
	void findLeafNode(std::unique_ptr<BarnesHut::Node>& node) {
		if (node == nullptr)return;
		std::cout << "������" << node->cmass << "λ��:" << node->mcenter.x << "," << node->mcenter.y << std::endl;

		if (node->isLeafNode()) {
			return;
		}
		for (int i = 0; i < 4; ++i)
			findLeafNode(node->ch[i]);
	}

	void test() {

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