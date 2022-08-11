#pragma once

//��������
//��֪�����������
//�ͽ�mesh��
//�Ż�ʱ����˼������AOS������SOA

#include <iostream>
#include <vector>

#include "../Vector2.hpp"

class Mesh2 {
public:
	Mesh2(std::initializer_list<Vector2D> vertexs, std::initializer_list<int> indexs) {
		vertexList.resize(vertexs.size());
		triList.resize(indexs.size() / 3);

		int count = 0;
		for (auto i = vertexs.begin(); i != vertexs.end(); ++i) {
			vertexList[count++] = *i;
		}
		count = 0;

		for (auto i = indexs.begin(); i != indexs.end(); i += 3) {
			triList[count++] = Triangle(*i, *(i + 1), *(i + 2));
		}

		originVertexList = vertexList;
	}

	struct Triangle {
		int index[3];
		Triangle() = default;
		Triangle(int i, int j, int k) {
			index[0] = i;
			index[1] = j;
			index[2] = k;
		}
	};

	Triangle& operator()(int i) {
		return triList[i];
	}

	Vector2D& operator()(int i, int j) {
		return vertexList[triList[i].index[j]];
	}

	Vector2D& at(int i, int j) {
		return vertexList[triList[i].index[j]];
	}

	int size()const {
		return triList.size();
	}

public:
	std::vector<Vector2D> vertexList;
	std::vector<Triangle> triList;
	std::vector<Vector2D> originVertexList;
};

typedef std::shared_ptr<Mesh2> Mesh2Ptr;
