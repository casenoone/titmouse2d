#include "SphSystemData2.h"
#include "SphPoly6Kernel2.h"

SphSystemData2::SphSystemData2() {

}


SphSystemData2::~SphSystemData2() {

}

ArrayD SphSystemData2::densities() {
	return _densities;
}

ArrayD SphSystemData2::pressures() {
	return _pressures;
}

void SphSystemData2::initDensity() {

	auto tempNeighbor = neighbor->neighBors();
	std::vector<double> tempData;
	auto tempPositions = positions();
	auto tempNumberOfParticles = numberOfParticles();
	//遍历每一个粒子

	//这里可作优化
	//SphStdKernel2 kernel(KERNEL_RADUIS);
	SphPolyKernel2 kernel(KERNEL_RADUIS);
	for (int i = 0; i < tempNumberOfParticles; ++i) {
		//遍历当前粒子的邻近粒子
		double tempDensities = 0;
		for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
			auto currentPosition = tempPositions[i];
			auto neighborPosition = tempPositions[*iter];
			double dist = currentPosition.dis(neighborPosition);
			double weight = MASS * kernel(dist);
			tempDensities += weight;
		}
		if (tempNeighbor[i].size() == 0) {
			tempDensities = 1.0;
		}
		tempData.push_back(tempDensities);
		//cout << tempDensities << endl;
	}

	_densities.set(tempData);

}

void SphSystemData2::updateDensities() {
	auto tempNeighbor = neighbor->neighBors();
	std::vector<double> tempData;

	auto tempPositions = positions();
	auto tempNumberOfParticles = numberOfParticles();
	auto cdensities = densities();
	//遍历每一个粒子

	//这里可作优化 将这个结构体对象写到SphData里
	SphStdKernel2 kernel(KERNEL_RADUIS);

	for (int i = 0; i < tempNumberOfParticles; ++i) {
		//遍历当前粒子的邻近粒子
		double tempDensities = 0;
		for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
			auto currentPosition = tempPositions[i];
			auto neighborPosition = tempPositions[*iter];
			double dist = currentPosition.dis(neighborPosition);
			double weight = MASS * kernel(dist);
			tempDensities += weight;
		}

		if (tempDensities == 0) {
			tempDensities = 0.1;
		}
		//cout << tempDensities << endl;
		tempData.push_back(tempDensities);
	}

	_densities.set(tempData);
}

void SphSystemData2::clearDensities() {

	auto size = _densities.dataSize();

	for (int i = 0; i < size; ++i) {
		_densities[i] = 0;
	}

}


Vector2D SphSystemData2::gradientAt(int i, ArrayD& values) {
	Vector2D sum;
	auto p = positions();
	auto d = densities();

	auto tempNeighbor = neighbor->neighBors();
	auto origin = p[i];

	//注意这里的kernel半径的值
	SphSpikyKernel2 kernel(KERNEL_RADUIS);

	for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
		auto j = *iter;
		auto neighborPosition = p[j];
		double dist = origin.dis(neighborPosition);
		if (dist > 0.0) {
			auto dir = (neighborPosition - origin) / dist;
			sum = sum + kernel.gradient(dist, dir) * (d[i] * MASS * (values[j] / (d[j] * d[j])));
		}
	}

	return sum;

}

double SphSystemData2::laplacianAt(int i, const ArrayD& values) {
	double sum = 0.0;
	auto p = positions();
	auto d = densities();
	auto tempNeighbor = neighbor->neighBors();
	auto origin = p[i];

	SphSpikyKernel2 kernel(KERNEL_RADUIS);

	for (auto iter = tempNeighbor[i].begin(); iter != tempNeighbor[i].end(); iter++) {
		auto j = *iter;
		auto neighborPosition = p[j];
		double dist = origin.dis(neighborPosition);
		sum += MASS * (values.lookAt(j) - values.lookAt(i)) / d[j] * kernel.secondDerivative(dist);
	}
	return sum;
}





