#include "MarchingCubes2.h"
#include "../../src/MathUtils.hpp"
#include "MarchingCubesTable2.h"

extern int triangleTable[16][4];

MarchingCube2::MarchingCube2() {

}



MarchingCube2::MarchingCube2(const Vector2I& resolution,
	const Vector2D& origin,
	double initialValue,
	double domainSizeX) {

	auto k = domainSizeX / resolution.x;
	auto m = Vector2D(k, k);

	data = std::make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);

}


MarchingCube2::~MarchingCube2() {

}

void MarchingCube2::setCircleList(std::vector<Circle>& circleList) {
	_circleList = circleList;
}


void MarchingCube2::setScalarField(const VertexCenteredScalarGrid2Ptr& _data) {
	data = _data;
}


//重载一个版本
//在这里需要计算法线，即sdf的梯度
void MarchingCube2::getLineSegmentSet(std::vector<SurfaceElement2>& SurfaceSet,
	const VertexCenteredScalarGrid2Ptr& _data) {


	//计算顶点势能
	setScalarField(_data);

	auto resolution = _data->resolution();

	Array2D num;

	//获取顶点配置
	getVoxelConfig(num);

	auto size = data->resolution();
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			auto currentNum = (int)num(i, j);
			auto inform = triangleTable[currentNum];
			for (int k = 0; k < 4; k += 2) {
				if (inform[k] != -1) {
					auto p1 = calculateIso(inform[k], i, j);
					auto p2 = calculateIso(inform[k + 1], i, j);
					SurfaceElement2 L;
					L.start = p1;
					L.end = p2;
					auto midPoint = 0.5 * (p1 + p2);
					auto tempN = _data->gradient(midPoint);
					L.normal = tempN.getNormalize();
					SurfaceSet.push_back(L);
				}

			}
		}
	}


}


void MarchingCube2::getLineSegmentSet(std::vector<LineSegment>& lineSet,
	const VertexCenteredScalarGrid2Ptr& _data) {


	//计算顶点势能
	setScalarField(_data);

	auto resolution = _data->resolution();

	Array2D num;

	//获取顶点配置
	getVoxelConfig(num);

	auto size = data->resolution();
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			auto currentNum = (int)num(i, j);
			auto inform = triangleTable[currentNum];
			for (int k = 0; k < 4; k += 2) {
				if (inform[k] != -1) {
					auto p1 = calculateIso(inform[k], i, j);
					auto p2 = calculateIso(inform[k + 1], i, j);
					LineSegment L;
					L.start = p1;
					L.End = p2;
					lineSet.push_back(L);
				}

			}
		}
	}


}



void MarchingCube2::getLineSegmentSet(std::vector<LineSegment>& lineSet) {

	//计算顶点势能
	calculateWeight();
	Array2D num;

	//获取顶点配置
	getVoxelConfig(num);

	auto size = data->resolution();
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			auto currentNum = (int)num(i, j);
			auto inform = triangleTable[currentNum];
			for (int k = 0; k < 4; k += 2) {
				if (inform[k] != -1) {
					auto p1 = calculateIso(inform[k], i, j);
					auto p2 = calculateIso(inform[k + 1], i, j);
					LineSegment L;
					L.start = p1;
					L.End = p2;
					lineSet.push_back(L);
				}

			}
		}
	}


}


void MarchingCube2::calculateWeight() {

	data->clearData(0.0);
	auto size = data->dataSize();
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {
			auto positionFunc = data->dataPosition();
			auto currentP = positionFunc(i, j);
			int circleNum = _circleList.size();

			for (int k = 0; k < circleNum; ++k) {
				//r^2/d^2
				(*data)(i, j) += sqr(_circleList[k].r) / currentP.disSquare(_circleList[k].center);
			}
		}
	}
}


void MarchingCube2::getVoxelConfig(Array2D& num) {

	auto size = data->resolution();
	num.reSize(size.x, size.y, 0.0);
	for (int j = 0; j < size.y; ++j) {
		for (int i = 0; i < size.x; ++i) {

			int nodeNum = 0;
			double value3 = (*data)(i, j);
			double value2 = (*data)(i + 1, j);
			double value1 = (*data)(i + 1, j + 1);
			double value0 = (*data)(i, j + 1);

			int temp_value = 0;

			if (value0 > temp_value) {
				value0 = 1;
			}
			else {
				value0 = 0;
			}

			if (value1 > temp_value) {
				value1 = 1;
			}
			else {
				value1 = 0;
			}

			if (value2 > temp_value) {
				value2 = 1;
			}
			else {
				value2 = 0;
			}

			if (value3 > temp_value) {
				value3 = 1;
			}
			else {
				value3 = 0;
			}



			nodeNum = static_cast<int>(value3 + value2 * 2 + value1 * 4 + value0 * 8);
			num(i, j) = nodeNum;
		}
	}
}




Vector2D MarchingCube2::calculateIso(int edge, int i, int j) {
	int start = edge;

	Vector2D p1, p2;
	Vector2D result;
	double value1, value2;
	auto posFunc = data->dataPosition();

	auto h = data->gridSpacing().x;

	switch (start)
	{

	case 0:
		p1 = posFunc(i, j + 1);
		p2 = posFunc(i + 1, j + 1);
		value1 = (*data)(i, j + 1);
		value2 = (*data)(i + 1, j + 1);
		break;
	case 1:
		p2 = posFunc(i + 1, j + 1);
		p1 = posFunc(i + 1, j);
		value2 = (*data)(i + 1, j + 1);
		value1 = (*data)(i + 1, j);
		break;
	case 2:
		p2 = posFunc(i + 1, j);
		p1 = posFunc(i, j);
		value2 = (*data)(i + 1, j);
		value1 = (*data)(i, j);
		break;
	case 3:
		p1 = posFunc(i, j);
		p2 = posFunc(i, j + 1);
		value1 = (*data)(i, j);
		value2 = (*data)(i, j + 1);
		break;
	default:
		break;
	}
	//这里这个比例公式突然发现有点别扭，和书上的不一样，两种都试试
	auto temp = (0 - value1) / (value2 - value1);

	//说明这是一条水平线
	if (fabs(p1.y - p2.y) < 0.0000001) {
		result.x = p1.x + h * temp;
		result.y = p1.y;

	}
	//说明这是一条竖直线
	else {
		result.x = p1.x;
		result.y = p1.y + h * temp;
	}
	return result;

}



MarchingCube2::Builder::Builder() {

}

MarchingCube2::Builder::~Builder() {

}

MarchingCube2::Builder MarchingCube2::builder() {
	return Builder();
}


MarchingCube2::Builder& MarchingCube2::Builder::withResolution(const Vector2I& resolution) {
	_resolution = resolution;
	return *this;
}

MarchingCube2::Builder& MarchingCube2::Builder::withResolution(int resolutionX, int resolutionY) {
	_resolution.x = resolutionX;
	_resolution.y = resolutionY;
	return *this;
}



MarchingCube2::Builder& MarchingCube2::Builder::withOrigin(const Vector2D& gridOrigin) {
	_gridOrigin = gridOrigin;
	return *this;
}


MarchingCube2::Builder& MarchingCube2::Builder::withOrigin(double gridOriginX, double gridOriginY) {
	_gridOrigin.x = gridOriginX;
	_gridOrigin.y = gridOriginY;
	return *this;
}


MarchingCube2::Builder& MarchingCube2::Builder::withInitialValue(double initialVal) {
	_initialVal = initialVal;
	return *this;
}


MarchingCube2::Builder& MarchingCube2::Builder::withDomainSizeX(double initialVal) {
	_domainSizeX = initialVal;
	return *this;
}





MarchingCube2Ptr MarchingCube2::Builder::makeShared() const {

	return std::shared_ptr<MarchingCube2>(
		new MarchingCube2(
			_resolution,
			_gridOrigin,
			_initialVal),
		[](MarchingCube2* obj) {
			delete obj;
		}
	);
}


MarchingCube2 MarchingCube2::Builder::build()const {

	return MarchingCube2(
		_resolution,
		_gridOrigin,
		_initialVal
	);

}