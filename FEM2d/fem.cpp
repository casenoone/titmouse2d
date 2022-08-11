#include <iostream>

#include "../titmouse2d/src/OtherMethod/FEM/FEMSolver2.h"
#include "../titmouse2d/src/mesh/Mesh2.h"



int main() {

	std::initializer_list<Vector2D> pos = { Vector2D(0,0),Vector2D(0,1),Vector2D(1,1),Vector2D(1,0) };
	std::initializer_list<int> index = { 0,1,2,0,2,3 };


	Mesh2 mesh(pos, index);

	auto vertexList = mesh.vertexList;
	for (auto i = vertexList.begin(); i != vertexList.end(); ++i) {
		//std::cout << i->x << " " << i->y << std::endl;
	}


	for (int i = 0; i < mesh.size(); ++i) {
		//std::cout << mesh(i, 2).x << " " << mesh(i, 2).y << std::endl;
	}

	return 0;
}