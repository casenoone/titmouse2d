#include "../../mesh/Mesh2.h"

class FEMData2 {
public:
	FEMData2(std::initializer_list<Vector2D> vertexs, std::initializer_list<int> indexs) {
		mesh = std::make_shared<Mesh2>(vertexs, indexs);
	}

public:
	Mesh2Ptr mesh;
};

typedef std::shared_ptr<FEMData2> FEMData2Ptr;
