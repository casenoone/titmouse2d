#include "NeighborSearcher2.h"
#include "../Size2.h"
#include "../ConstVar.h"
NeighborSearcher2::NeighborSearcher2() :_resolutionX(0), _resolutionY(0), _numberOfParticles(0) {}

NeighborSearcher2::~NeighborSearcher2() {

}



void NeighborSearcher2::resetDataSize() {

	_data.resize(_numberOfParticles);

}

NeighborSearcher2::NeighborSearcher2(int resolutionX, int resolutionY, int numberOfParticles)
	:_resolutionX(resolutionX), _resolutionY(resolutionY), _numberOfParticles(numberOfParticles) {
	resetDataSize();
}

void NeighborSearcher2::clearData() {
	_data.clear();
}

bool NeighborSearcher2::IsXoverBorder(int idx) {
	if (idx < 0 || idx >= _resolutionX) {
		return true;
	}
	return false;
}

bool NeighborSearcher2::IsYoverBorder(int idx) {
	if (idx < 0 || idx >= _resolutionY) {
		return true;
	}
	return false;
}

bool NeighborSearcher2::IsNeighbor(Vector2D& pos1, Vector2D& pos2, const double& r) {
	auto dis = pos1.dis(pos2);
	if (dis < r) {
		return true;
	}
	return false;
}


void NeighborSearcher2::forEachNeighborGrid(Vector2I& idx, int particleId, std::vector<std::vector<std::vector<int>>>& grids, const Array<Vector2D>& positions) {

	//idx是待求格子的坐标
	int x = idx.x;
	int y = idx.y;


	if (!IsYoverBorder(y) && !IsXoverBorder(x)) {
		//如果该待求格子存在，那么取该格子里面的所有粒子，即currentParticles
		auto currentParticles = grids[x][y];

		for (auto iter = currentParticles.begin(); iter != currentParticles.end(); iter++) {
			auto pos1 = positions.lookAt(*iter);
			auto pos2 = positions.lookAt(particleId);
			if (true/**iter != particleId*/) {
				double r = 0.15;
				if (IsNeighbor(pos1, pos2, r) == true) {
					_data[particleId].push_back(*iter);
				}
			}

		}
	}

}

void NeighborSearcher2::forEachNeighborGrid(double r, Vector2I& idx, int particleId, std::vector<std::vector<std::vector<int>>>& grids, const Array<Vector2D>& positions) {

	//idx是待求格子的坐标
	int x = idx.x;
	int y = idx.y;


	if (!IsYoverBorder(y) && !IsXoverBorder(x)) {
		//如果该待求格子存在，那么取该格子里面的所有粒子，即currentParticles
		auto currentParticles = grids[x][y];

		for (auto iter = currentParticles.begin(); iter != currentParticles.end(); iter++) {
			auto pos1 = positions.lookAt(*iter);
			auto pos2 = positions.lookAt(particleId);
			if (true/**iter != particleId*/) {
				if (IsNeighbor(pos1, pos2, r) == true) {
					_data[particleId].push_back(*iter);
				}
			}

		}
	}

}


void NeighborSearcher2::setNeiborList(double r, const Array<Vector2D>& positions) {

	clearData();
	resetDataSize();

	//grids里保存粒子映射后的编号
	std::vector<std::vector<std::vector<int>>> grids;

	//gridPositions保存了粒子的映射坐标
	std::vector<Vector2I> gridPositons;
	//对格子进行初始化
	grids.resize(_resolutionX);
	for (auto iter = grids.begin(); iter != grids.end(); iter++) {
		iter->resize(_resolutionY);
	}

	int particleIdx = 0;

	positions.forEachIndex([&](int i) {

		double maxWidth = 2;
		double maxHeight = 2;
		double tempX = positions.lookAt(i).x;
		double tempY = positions.lookAt(i).y;

		double scaleX = maxWidth / _resolutionX;
		double scaleY = maxHeight / _resolutionY;

		int gridX = floor(tempX / scaleX);
		int gridY = floor(tempY / scaleY);
		int k = 1;

		if (gridX >= _resolutionX) {
			gridX = _resolutionX - k;
		}

		if (gridX < 0) {
			gridX = 0;
		}

		if (gridY >= _resolutionY) {
			gridY = _resolutionY - k;
		}

		if (gridY < 0) {
			gridY = 0;
		}
		Vector2I pos(gridX, gridY);
		gridPositons.push_back(pos);

		grids[gridX][gridY].push_back(particleIdx);
		particleIdx++;

		});



	particleIdx = 0;

	positions.forEachIndex([&](int i) {

		auto current = gridPositons[particleIdx];
		Vector2I top(current.x, current.y + 1);
		Vector2I down(current.x, current.y - 1);
		Vector2I left(current.x - 1, current.y);
		Vector2I right(current.x + 1, current.y);
		Vector2I leftTop(current.x - 1, current.y + 1);
		Vector2I rightTop(current.x + 1, current.y + 1);
		Vector2I leftDown(current.x - 1, current.y - 1);
		Vector2I rightDown(current.x + 1, current.y - 1);

		forEachNeighborGrid(r, current, particleIdx, grids, positions);
		forEachNeighborGrid(r, top, particleIdx, grids, positions);
		forEachNeighborGrid(r, down, particleIdx, grids, positions);
		forEachNeighborGrid(r, left, particleIdx, grids, positions);
		forEachNeighborGrid(r, right, particleIdx, grids, positions);
		forEachNeighborGrid(r, leftTop, particleIdx, grids, positions);
		forEachNeighborGrid(r, rightTop, particleIdx, grids, positions);
		forEachNeighborGrid(r, leftDown, particleIdx, grids, positions);
		forEachNeighborGrid(r, rightDown, particleIdx, grids, positions);

		particleIdx++;

		});


}





void NeighborSearcher2::setNeiborList(const Array<Vector2D>& positions) {
	clearData();
	resetDataSize();



	//grids里保存粒子映射后的编号
	std::vector<std::vector<std::vector<int>>> grids;

	//gridPositions保存了粒子的映射坐标
	std::vector<Vector2I> gridPositons;
	//对格子进行初始化
	grids.resize(_resolutionX);
	for (auto iter = grids.begin(); iter != grids.end(); iter++) {
		iter->resize(_resolutionY);
	}

	int particleIdx = 0;

	positions.forEachIndex([&](int i) {

		double maxWidth = 2;
		double maxHeight = 2;
		double tempX = positions.lookAt(i).x;
		double tempY = positions.lookAt(i).y;

		double scaleX = maxWidth / _resolutionX;
		double scaleY = maxHeight / _resolutionY;

		int gridX = floor(tempX / scaleX);
		int gridY = floor(tempY / scaleY);
		int k = 1;

		if (gridX >= _resolutionX) {
			gridX = _resolutionX - k;
		}

		if (gridX < 0) {
			gridX = 0;
		}

		if (gridY >= _resolutionY) {
			gridY = _resolutionY - k;
		}

		if (gridY < 0) {
			gridY = 0;
		}
		Vector2I pos(gridX, gridY);
		gridPositons.push_back(pos);

		grids[gridX][gridY].push_back(particleIdx);
		particleIdx++;

		});



	particleIdx = 0;

	positions.forEachIndex([&](int i) {

		auto current = gridPositons[particleIdx];
		Vector2I top(current.x, current.y + 1);
		Vector2I down(current.x, current.y - 1);
		Vector2I left(current.x - 1, current.y);
		Vector2I right(current.x + 1, current.y);
		Vector2I leftTop(current.x - 1, current.y + 1);
		Vector2I rightTop(current.x + 1, current.y + 1);
		Vector2I leftDown(current.x - 1, current.y - 1);
		Vector2I rightDown(current.x + 1, current.y - 1);

		forEachNeighborGrid(current, particleIdx, grids, positions);
		forEachNeighborGrid(top, particleIdx, grids, positions);
		forEachNeighborGrid(down, particleIdx, grids, positions);
		forEachNeighborGrid(left, particleIdx, grids, positions);
		forEachNeighborGrid(right, particleIdx, grids, positions);
		forEachNeighborGrid(leftTop, particleIdx, grids, positions);
		forEachNeighborGrid(rightTop, particleIdx, grids, positions);
		forEachNeighborGrid(leftDown, particleIdx, grids, positions);
		forEachNeighborGrid(rightDown, particleIdx, grids, positions);

		particleIdx++;

		});


}

std::vector<std::vector<int>>& NeighborSearcher2::neighBors() {
	return _data;
}