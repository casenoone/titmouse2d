#ifndef NEIGHBORSEARCHER2_H
#define NEIGHBORSEARCHER2_H

#include <iostream>
using namespace std;

#include <vector>
#include "../Vector2.hpp"
#include "../ArrayPtr.hpp"
#include "../Array2Ptr.hpp"

//Íø¸ñËÑË÷·¨

class NeighborSearcher2 {
public:
	NeighborSearcher2();
	~NeighborSearcher2();

	explicit NeighborSearcher2(size_t resolutionX, size_t resolutionY, size_t numberOfParticles);

	void setNeiborList(const ArrayPtr<Vector2<double>>& positions);

	vector<vector<size_t>>& neighBors();

private:

	size_t _resolutionX;
	size_t _resolutionY;
	size_t _numberOfParticles;

	vector<vector<size_t>> _data;

	void resetDataSize();

	void clearData();

	bool IsXoverBorder(size_t idx);

	bool IsYoverBorder(size_t idx);

	bool IsNeighbor(Vector2 <double> & pos1, Vector2<double>& pos2, const double& r);

	void forEachNeighborGrid(Vector2<size_t>& idx, size_t particleId, vector<vector<vector<size_t>>>& grids, const ArrayPtr<Vector2<double>>& positions);

};

using NeighborSearcher2Ptr = shared_ptr<NeighborSearcher2>;


#endif