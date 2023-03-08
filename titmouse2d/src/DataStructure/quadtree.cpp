#include "quadtree.h"

//将粒子映射到四叉树最细分辨率网格中
void QuadTree::buildMapGrid() {
	int particleIdx = 0;
	for (int i = 0; i < pos.size(); ++i) {
		double maxWidth = 2;
		double maxHeight = 2;
		double tempX = pos[i].x;
		double tempY = pos[i].y;

		double scaleX = maxWidth / _minRes.x;
		double scaleY = maxHeight / _minRes.y;

		int gridX = floor(tempX / scaleX);
		int gridY = floor(tempY / scaleY);
		int k = 1;

		if (gridX >= _minRes.x)
			gridX = _minRes.x - k;
		if (gridX < 0)
			gridX = 0;
		if (gridY >= _minRes.y)
			gridY = _minRes.y - k;
		if (gridY < 0)
			gridY = 0;

		mapGrid[gridX][gridY].push_back(i);
		//std::cout << gridX << "," << gridY << std::endl;
	}
}


void QuadTree::build(
	std::unique_ptr<QuadTree::Node>& node,
	int xl, int xr, int yd, int yu) {

	if (node == nullptr)return;

	if ((xr - xl) < 2) return;

	//为子结点分配内存
	for (int i = 0; i < 4; ++i) {
		node->ch[i] = std::make_unique<QuadTree::Node>();
	}

	//当xl-xr等于2时，达到最细分辨率
	//确定叶子结点坐标
	if ((xr - xl) == 2) {
		node->ch[0]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl, yd));
		node->ch[1]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl + 1, yd));
		node->ch[2]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl, yd + 1));
		node->ch[3]->gridIdx = std::make_unique<Vector2I>(Vector2I(xl + 1, yd + 1));

		for (int t = 0; t < 4; ++t) {
			int i = node->ch[t]->gridIdx->x;
			int j = node->ch[t]->gridIdx->y;
			auto& particlesList = mapGrid[i][j];
			if (particlesList.size() == 0)continue;

			//std::cout << particlesList.size() << std::endl;
			for (int k = 0; k < particlesList.size(); ++k) {
				//目前假设粒子质量均为1
				node->ch[t]->cmass += 1;
			}

			double temp_x = 0;
			double temp_y = 0;
			for (int k = 0; k < particlesList.size(); ++k) {
				temp_x += pos[k].x;
				temp_y += pos[k].y;
			}
			temp_x /= node->ch[t]->cmass;
			temp_y /= node->ch[t]->cmass;
			node->ch[t]->mcenter = Vector2D(temp_x, temp_y);

		}
	}

	double temp_mass = 0;
	double temp_x = 0;
	double temp_y = 0;
	for (int i = 0; i < 4; ++i) {
		temp_mass += node->ch[i]->cmass;
		temp_x += node->ch[i]->mcenter.x;
		temp_y += node->ch[i]->mcenter.y;
	}
	if (temp_mass != 0) {
		temp_x /= temp_mass;
		temp_y /= temp_mass;
		node->cmass = temp_mass;
		node->mcenter = Vector2D(temp_x, temp_y);
	}
	//std::cout << "sdsds" << std::endl;

	build(node->ch[0], xl, (xl + xr) * 0.5, yd, (yd + yu) * 0.5);
	build(node->ch[1], (xl + xr) * 0.5, xr, yd, (yd + yu) * 0.5);
	build(node->ch[2], xl, (xl + xr) * 0.5, (yd + yu) * 0.5, yu);
	build(node->ch[3], (xl + xr) * 0.5, xr, (yd + yu) * 0.5, yu);
}
