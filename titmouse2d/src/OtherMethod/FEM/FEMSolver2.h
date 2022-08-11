#include "FEMData2.h"

class FEMSolver2 {
public:
	FEMSolver2(std::initializer_list<Vector2D> vertexs,
		std::initializer_list<int> indexs) {
		femData = std::make_shared<FEMData2>(vertexs, indexs);
	}



public:
	FEMData2Ptr femData;
};

