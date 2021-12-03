#include "MarchingCubes2.h"
#include "../../src/MathUtils.hpp"
#include "MarchingCubesTable2.h"

extern int triangleTable[16][4];

MarchingCube2::MarchingCube2() {

}



MarchingCube2::MarchingCube2(const Vector2<size_t>& resolution,
	const Vector2<double>& origin,
	double initialValue,
	double domainSizeX) {

	auto k = domainSizeX / resolution.x;
	auto m = Vector2<double>(k, k);

	data = make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);

}


MarchingCube2::~MarchingCube2() {

}

void MarchingCube2::setCircleList(vector<Circle>& circleList) {
	_circleList = circleList;
}


void MarchingCube2::setScalarField(const VertexCenteredScalarGrid2Ptr& _data) {
	data = _data;
}


void MarchingCube2::getLineSegmentSet(vector<LineSegment>& lineSet, 
	const VertexCenteredScalarGrid2Ptr& _data) {

	//计算顶点势能
	setScalarField(_data);

	Array2Ptr<double> num;

	//获取顶点配置
	getVoxelConfig(num);

	auto size = data->resolution();
	for (size_t j = 0; j < size.y; ++j) {
		for (size_t i = 0; i < size.x; ++i) {
			auto currentNum = (size_t)num(i, j);
			auto inform = triangleTable[currentNum];
			for (size_t k = 0; k < 4; k += 2) {
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



void MarchingCube2::getLineSegmentSet(vector<LineSegment>& lineSet) {

	//计算顶点势能
	calculateWeight();

	Array2Ptr<double> num;

	//获取顶点配置
	getVoxelConfig(num);

	auto size = data->resolution();
	for (size_t j = 0; j < size.y; ++j) {
		for (size_t i = 0; i < size.x; ++i) {
			auto currentNum = (size_t)num(i, j);
			auto inform = triangleTable[currentNum];
			for (size_t k = 0; k < 4; k += 2) {
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
	for (size_t j = 0; j < size.y; ++j) {
		for (size_t i = 0; i < size.x; ++i) {
			auto positionFunc = data->dataPosition();
			auto currentP = positionFunc(i, j);
			//cout << positionFunc(20, 20).x() << "," << positionFunc(20, 20).y() << endl;
			auto circleNum = _circleList.size();
			for (size_t k = 0; k < circleNum; ++k) {
				//r^2/d^2
				(*data)(i, j) += sqr(_circleList[k].r) / currentP.disSquare(_circleList[k].center);
			}
		}
	}
}


void MarchingCube2::getVoxelConfig(Array2Ptr<double>& num) {

	auto size = data->resolution();
	num.reSize(size.x, size.y, 0.0);
	for (size_t j = 0; j < size.y; ++j) {
		for (size_t i = 0; i < size.x; ++i) {

			size_t nodeNum = 0;
			double value3 = (*data)(i, j);
			double value2 = (*data)(i + 1, j);
			double value1 = (*data)(i + 1, j + 1);
			double value0 = (*data)(i, j + 1);
			if (value0 > 1) {
				value0 = 1;
			}
			else {
				value0 = 0;
			}

			if (value1 > 1) {
				value1 = 1;
			}
			else {
				value1 = 0;
			}

			if (value2 > 1) {
				value2 = 1;
			}
			else {
				value2 = 0;
			}

			if (value3 > 1) {
				value3 = 1;
			}
			else {
				value3 = 0;
			}



			nodeNum = value3 + value2 * 2 + value1 * 4 + value0 * 8;
			num(i, j) = nodeNum;
		}
	}
}




Vector2<double> MarchingCube2::calculateIso(size_t edge, size_t i, size_t j) {
	int start = edge;

	Vector2<double> p1, p2;
	Vector2<double> result;
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
	auto temp = (1 - value1) / (value2 - value1);
	
	//auto temp = (value1) / (value1 - value2);

	//说明这是一条水平线
	if (temp < 0)cout << temp << endl;

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


MarchingCube2::Builder& MarchingCube2::Builder::withResolution(const Vector2<size_t>& resolution) {
	_resolution = resolution;
	return *this;
}

MarchingCube2::Builder& MarchingCube2::Builder::withResolution(size_t resolutionX, size_t resolutionY) {
	_resolution.x = resolutionX;
	_resolution.y = resolutionY;
	return *this;
}



MarchingCube2::Builder& MarchingCube2::Builder::withOrigin(const Vector2<double>& gridOrigin) {
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