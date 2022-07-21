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

		//��ǵ�ǰ�ڵ������ĵڼ���
		int level = 0;
	};


public:
	//virtual void generate() = 0;
	void generate() {

	}
public:
	QuadTree() {
	}

	//���ĸ��ڵ�
	std::shared_ptr<Node> root;

	//�����ܲ���
	int totalLevel = 1;


};