#ifndef NEIGHBORSEARCHER2_H
#define NEIGHBORSEARCHER2_H

#include <iostream>
using namespace std;

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

	void setNeiborList(const Array<Vector2<double>>& positions);

	vector<vector<int>>& neighBors();

private:

	int _resolutionX;
	int _resolutionY;
	int _numberOfParticles;

	vector<vector<int>> _data;

	void resetDataSize();

	void clearData();

	bool IsXoverBorder(int idx);

	bool IsYoverBorder(int idx);

	bool IsNeighbor(Vector2 <double>& pos1, Vector2<double>& pos2, const double& r);

	void forEachNeighborGrid(Vector2<int>& idx, int particleId, vector<vector<vector<int>>>& grids, const Array<Vector2<double>>& positions);

};

using NeighborSearcher2Ptr = shared_ptr<NeighborSearcher2>;


#endif