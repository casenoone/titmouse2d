#pragma once
#include <iostream>


class QuadTree {
public:
	class Node {
	public:
		Node() = default;
		virtual ~Node() {

		}
	public:
		std::unique_ptr<Node> p1 = nullptr;
		std::unique_ptr<Node> p2 = nullptr;
		std::unique_ptr<Node> p3 = nullptr;
		std::unique_ptr<Node> p4 = nullptr;

		//标记当前节点在树的第几层
		int level = 0;
	};


public:
	//virtual void generate() = 0;
	void generate() {

	}
public:
	QuadTree() {
	}

	//树的根节点
	std::shared_ptr<Node> root;

	//树的总层数
	int totalLevel = 1;


};