#pragma once

#include <iostream>
#include "../../Vector2.hpp"
#include "FVMData2.h"

class FVMSolver2 {
public:
	FVMSolver2(std::initializer_list<Vector2D> vertexs,
		std::initializer_list<int> indexs) {
		fvmData = std::make_shared<FVMData2>(vertexs, indexs);
	}

public:
	FVMData2Ptr fvmData;
};