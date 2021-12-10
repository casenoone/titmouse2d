#ifndef ADVECTIONSOLVER2_H
#define ADVECTIONSOLVER2_H

#include "FaceCenteredGrid2.h"
#include "../Eulerian/CellCenteredVectorGrid2.hpp"
#include "../MathUtils.hpp"
class AdvectionSolver2 {
public:
	AdvectionSolver2();

	~AdvectionSolver2();

	template<typename T>
	void solve(const FaceCenteredGrid2Ptr& flow, CellCenteredVectorGrid2Ptr<T> advectedData, double timeIntervalInSeconds);

public:

};


template<typename T>
void AdvectionSolver2::solve(const FaceCenteredGrid2Ptr& flow, CellCenteredVectorGrid2Ptr<T> advectedData, double timeIntervalInSeconds) {
	auto resolution = advectedData->resolution();
	auto data = advectedData->datas();
	auto gridSpacing = advectedData->gridSpacing();

	Array2Ptr<T> newData;
	//newData = data;
	newData.reSize(resolution.x, resolution.y, T());

	for (int i = 0; i < resolution.x; ++i) {
		for (int j = 0; j < resolution.y; ++j) {
			auto posFunc = advectedData->dataPosition();
			auto current_pos = posFunc(i, j);
			auto current_vec = flow->sample(current_pos);
			auto backTracedPoint = current_pos - current_vec * timeIntervalInSeconds;

			if (backTracedPoint.x >= 0 && backTracedPoint.x <= 2 && backTracedPoint.y >= 0 && backTracedPoint.y <= 2) {
				newData(i, j) = advectedData->sample(backTracedPoint);

			}
			else {

				double x, y;
				x = clamp<double>(backTracedPoint.x, 0.0, 2.0);
				y = clamp<double>(backTracedPoint.y, 0.0, 2.0);
				newData(i, j) = advectedData->sample(Vector2<double>(x, y));
			}


		}
	}

	data = newData;
}


#endif