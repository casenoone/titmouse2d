#include "NeighborSearcher2.h"
#include "../Size2.h"
#include "../ConstVar.h"
NeighborSearcher2::NeighborSearcher2() :_resolutionX(0), _resolutionY(0), _numberOfParticles(0) {}

NeighborSearcher2::~NeighborSearcher2() {

}



void NeighborSearcher2::resetDataSize() {

	_data.resize(_numberOfParticles);

}

NeighborSearcher2::NeighborSearcher2(size_t resolutionX, size_t resolutionY, size_t numberOfParticles)
	:_resolutionX(resolutionX), _resolutionY(resolutionY), _numberOfParticles(numberOfParticles) {
	resetDataSize();
}

void NeighborSearcher2::clearData() {
	_data.clear();
}

bool NeighborSearcher2::IsXoverBorder(size_t idx) {
	if (idx < 0 || idx >= _resolutionX) {
		return true;
	}
	return false;
}

bool NeighborSearcher2::IsYoverBorder(size_t idx) {
	if (idx < 0 || idx >= _resolutionY) {
		return true;
	}
	return false;
}

bool NeighborSearcher2::IsNeighbor(Vector2<double>& pos1, Vector2<double>& pos2, const double& r) {
	auto dis = pos1.dis(pos2);
	if (dis < r) {
		return true;
	}
	return false;
}


void NeighborSearcher2::forEachNeighborGrid(Vector2<size_t>& idx, size_t particleId, vector<vector<vector<size_t>>>& grids, const ArrayPtr<Vector2<double>>& positions) {

	//idx是待求格子的坐标
	size_t x = idx.x;
	size_t y = idx.y;


	if (!IsYoverBorder(y) && !IsXoverBorder(x)) {
		//如果该待求格子存在，那么取该格子里面的所有粒子，即currentParticles
		auto currentParticles = grids[x][y];

		for (auto iter = currentParticles.begin(); iter != currentParticles.end(); iter++) {
			auto pos1 = positions.lookAt(*iter);
			auto pos2 = positions.lookAt(particleId);
			if (*iter != particleId) {
				if (IsNeighbor(pos1, pos2, R) == true) {
					_data[particleId].push_back(*iter);
				}
			}

		}
	}

}

void NeighborSearcher2::setNeiborList(const ArrayPtr<Vector2<double>>& positions) {
	clearData();
	resetDataSize();



	//grids里保存粒子映射后的编号
	vector<vector<vector<size_t>>> grids;

	//gridPositions保存了粒子的映射坐标
	vector<Vector2<size_t>> gridPositons;
	//对格子进行初始化
	grids.resize(_resolutionX);
	for (auto iter = grids.begin(); iter != grids.end(); iter++) {
		iter->resize(_resolutionY);
	}

	int particleIdx = 0;
	
	positions.forEachIndex([&](size_t i) {
	
		double maxWidth = 2;
		double maxHeight = 2;
		double tempX = positions.lookAt(i).x;
		double tempY = positions.lookAt(i).y;

		double scaleX = maxWidth / _resolutionX;
		double scaleY = maxHeight / _resolutionY;

		size_t gridX = floor(tempX / scaleX);
		size_t gridY = floor(tempY / scaleY);
		size_t k = 1;

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
		Vector2<size_t> pos(gridX, gridY);
		gridPositons.push_back(pos);

		grids[gridX][gridY].push_back(particleIdx);
		particleIdx++;
		
	});



	particleIdx = 0;
	
	positions.forEachIndex([&](size_t i) {
		
		auto current = gridPositons[particleIdx];
		Vector2<size_t> top(current.x, current.y + 1);
		Vector2<size_t> down(current.x, current.y - 1);
		Vector2<size_t> left(current.x - 1, current.y);
		Vector2<size_t> right(current.x + 1, current.y);
		Vector2<size_t> leftTop(current.x - 1, current.y + 1);
		Vector2<size_t> rightTop(current.x + 1, current.y + 1);
		Vector2<size_t> leftDown(current.x - 1, current.y - 1);
		Vector2<size_t> rightDown(current.x + 1, current.y - 1);

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

vector<vector<size_t>>& NeighborSearcher2::neighBors() {
	return _data;
}