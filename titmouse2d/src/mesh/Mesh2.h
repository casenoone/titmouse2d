#pragma once

//三角网格
//不知道该如何命名
//就叫mesh吧
//优化时可以思考是用AOS还是用SOA

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