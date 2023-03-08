#include <iostream>
using namespace std;
#include <vector>

#include "../titmouse2d/src/DataStructure/quadtree.h"
#include "../titmouse2d/src/Vector2.hpp"

int main() {


	vector<Vector2D> pos;
	for (int i = 0; i < 5; ++i) {
		pos.push_back(Vector2D(i * 0.1 + 0.1, i * 0.2));
	}

	QuadTree tree(Vector2I(4, 4), pos);

	tree.findLeafNode(tree.root);

	return 0;
}