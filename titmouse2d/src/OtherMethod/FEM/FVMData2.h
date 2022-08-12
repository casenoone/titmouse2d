#pragma once
#include "../../mesh/Mesh2.h"
#include "../../Matrix2x2.hpp"

class FVMData2 {
public:
	FVMData2(std::initializer_list<Vector2D> vertexs, std::initializer_list<int> indexs) {

	}
};

typedef std::shared_ptr<FVMData2> FVMData2Ptr;
