#pragma once
#include "../../DataStructure/quadtree.h"

class VortexTreeCode : public QuadTree {
public:
	class VNode : public QuadTree::Node {
	public:
		VNode() {

		}
	public:
		double Qk = 0.0;
		double Q = 5.0;

	};

public:
	VortexTreeCode() {
		root = std::make_shared<VNode>();
		vRoot = std::make_shared<VNode>();
		vRoot = std::dynamic_pointer_cast<VNode>(root);
	}

public:
	std::shared_ptr<VNode> vRoot;
};