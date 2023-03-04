#pragma once
#include <iostream>
#include <vector>
#include "Geometry2.h"
#include "Ray2.h"
#include "BVH2.h"

class IntersectTestSystem {
public:
	IntersectTestSystem(std::vector<Geometry2Ptr>& data_) :bvh(data_), data(data_) {
		bvh.buildBVH();
	}

	std::vector<size_t> findHitObj(const Ray2& ray)const {
		std::vector<size_t> list;
		recusiveQuery(bvh.node, ray, list);
		return list;
	}

	std::vector<size_t> findHitObj_Violent(const Ray2& ray)const {
		std::vector<size_t> list;
		for (int i = 0; i < data.size(); ++i) {
			if (ray.IsInTersect(data[i]->box)) {
				if (data[i]->IsInTersect(ray)) {
					list.push_back(data[i]->number);
				}
			}
		}
		return list;
	}

private:
	void recusiveQuery(const std::unique_ptr<BVH2::treeNode>& node,
		const Ray2& ray,
		std::vector<size_t>& list)const {

		if (!node) { return; }

		if (ray.IsInTersect(node->box)) {
			if (node->isLeaf()) {
				auto& address = node->pAdress;
				for (auto i : *address) {
					if (ray.IsInTersect(i->box)) {
						if (i->IsInTersect(ray)) {
							list.push_back(i->number);
						}
					}
				}
			}
			recusiveQuery(node->child0, ray, list);
			recusiveQuery(node->child1, ray, list);
		}
	}
public:
	BVH2 bvh;

	std::vector<Geometry2Ptr>& data;
};