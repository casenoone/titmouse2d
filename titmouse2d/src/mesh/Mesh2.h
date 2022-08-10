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
	Mesh2();

	Mesh2(std::initializer_list<Vector2D> vertexs, std::initializer_list<int> indexs) {
		vertexList.resize(vertexs.size());
		triList.resize(indexs.size() * 3);


	}

	struct Triangle {
		int index[3];
	};

	Triangle& operator()(int i) {
		return triList[i];
	}

	Vector2D& operator()(int i, int j) {
		return vertexList[triList[i].index[j]];
	}

public:
	std::vector<Vector2D> vertexList;
	std::vector<Triangle> triList;
};