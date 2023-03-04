#pragma once
#include <iostream>
#include <algorithm>
#include <vector>

#include "BoundingBox2.h"
#include "Sphere2.h"
#include "Geometry2.h"

class BVH2 {
public:
	BVH2(const std::vector<Geometry2Ptr>& obj) {
		pdata.resize(obj.size());
		for (int i = 0; i < obj.size(); ++i) {
			pdata[i] = obj[i];
		}
	}

	class treeNode {
	public:
		treeNode() = default;
		bool isLeaf() const {
			if ((!child0) && (!child1))return true;
			return false;
		}
	public:
		//bvh������
		std::unique_ptr<treeNode> child0;
		//bvh���Һ���
		std::unique_ptr<treeNode> child1;

		std::unique_ptr<std::vector<Geometry2Ptr>> pAdress;
		//ָ������ʵ������ĵ�ַ
		//ÿ�����İ�Χ��
		BoundingBox2 box;
	};

	//�ݹ鹹��BVH��
	void buildBVH() {
		build(pdata, node);
	}

	//�ݹ鹹��BVH��
	void build(std::vector<Geometry2Ptr>& objList,
		std::unique_ptr<BVH2::treeNode>& this_node) {

		if (objList.size() <= min_num) {

			this_node = std::make_unique<treeNode>();
			this_node->box = getBoudingBox(objList);

			this_node->pAdress = std::make_unique<std::vector<Geometry2Ptr>>();
			for (auto i = 0; i < objList.size(); ++i) {
				this_node->pAdress->push_back(objList[i]);
			}
		}
		else {

			//Ϊ��ǰ�ڵ�����ڴ�
			this_node = std::make_unique<treeNode>();
			//���Χ��
			this_node->box = getBoudingBox(objList);
			//����������
			if (divideMode(this_node->box) == X) {
				//����x�����������
				std::sort(objList.begin(), objList.end(), [&](Geometry2Ptr s1, Geometry2Ptr s2)->bool {
					if (s1->barycenter().x < s2->barycenter().x)return true;
					return false;
					});
				auto vec0 = std::vector<Geometry2Ptr>(objList.begin(), objList.begin() + objList.size() / 2);
				auto vec1 = std::vector<Geometry2Ptr>(objList.begin() + objList.size() / 2, objList.end());

				build(vec0, this_node->child0);
				build(vec1, this_node->child1);

			}
			else {
				//����y�����������
				std::sort(objList.begin(), objList.end(), [&](Geometry2Ptr s1, Geometry2Ptr s2)->bool {
					if (s1->barycenter().y < s2->barycenter().y)return true;
					return false;
					});

				auto vec0 = std::vector<Geometry2Ptr>(objList.begin(), objList.begin() + objList.size() / 2);
				auto vec1 = std::vector<Geometry2Ptr>(objList.begin() + objList.size() / 2, objList.end());

				build(vec0, this_node->child0);
				build(vec1, this_node->child1);
			}

		}
	}

private:
	//������Χ��
	BoundingBox2 getBoudingBox(const std::vector<Geometry2Ptr>& objList)const {
		auto min_x = objList[0]->box.lower.x;
		auto min_y = objList[0]->box.lower.y;
		auto max_x = min_x;
		auto max_y = min_y;

		for (int i = 0; i < objList.size(); ++i) {
			auto& box = objList[i]->box;
			if (min_x > box.lower.x)
				min_x = box.lower.x;
			if (min_y > box.lower.y)
				min_y = box.lower.y;
			if (max_x < box.upper.x)
				max_x = box.upper.x;
			if (max_y < box.upper.y)
				max_y = box.upper.y;
		}
		return { Vector2D(min_x,min_y),Vector2D(max_x,max_y) };
	}

	//�ж�ˮƽ���ֻ�����ֱ����
	char divideMode(const BoundingBox2& box)const {
		if (std::abs(box.lower.x - box.upper.x) > std::abs(box.lower.y - box.upper.y))return X;
		return Y;
	}
public:
	std::unique_ptr<treeNode> node;
	std::vector<Geometry2Ptr> pdata;

public:
	//���������������С��min_numʱֹͣ���Ļ���
	int min_num = 3;

	//��ʶˮƽ�л�����ֱ��
	static const char X = 0;
	static const char Y = 1;
};