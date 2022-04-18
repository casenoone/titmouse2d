#ifndef NEIGHBORSEARCHER2_H
#define NEIGHBORSEARCHER2_H

#include <iostream>

#include <vector>
#include "../Vector2.hpp"
#include "../Array.hpp"
#include "../Array2.hpp"

//Íø¸ñËÑË÷·¨

class NeighborSearcher2 {
public:
	NeighborSearcher2();
	~NeighborSearcher2();

	explicit NeighborSearcher2(int resolutionX, int resolutionY, int numberOfParticles);

	void setNeiborList(const Array<Vector2D>& positions);

	void setNeiborList(double r, const Array<Vector2D>& positions);

	std::vector<std::vector<int>>& neighBors();

private:

	int _resolutionX;
	int _resolutionY;
	int _numberOfParticles;

	std::vector<std::vector<int>> _data;

	void resetDataSize();

	void clearData();

	bool IsXoverBorder(int idx);

	bool IsYoverBorder(int idx);

	bool IsNeighbor(Vector2 <double>& pos1, Vector2D& pos2, const double& r);

	void forEachNeighborGrid(Vector2I& idx, int particleId, std::vector<std::vector<std::vector<int>>>& grids, const Array<Vector2D>& positions);

	void forEachNeighborGrid(double r, Vector2I& idx, int particleId, std::vector<std::vector<std::vector<int>>>& grids, const Array<Vector2D>& positions);

};

using NeighborSearcher2Ptr = std::shared_ptr<NeighborSearcher2>;


#endif