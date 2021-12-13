#include "AdvectionSolver2.h"

AdvectionSolver2::AdvectionSolver2() {

}

AdvectionSolver2::~AdvectionSolver2() {

}


void AdvectionSolver2::solve(const FaceCenteredGrid2Ptr& flow,
	VertexCenteredScalarGrid2Ptr advectedData,
	double timeIntervalInSeconds) {

	auto dataSize = advectedData->dataSize();
	auto data = advectedData->datas();
	auto gridSpacing = advectedData->gridSpacing();

	Array2Ptr<double> newData;
	//newData = data;
	newData.reSize(dataSize.x, dataSize.y, 0.0);

	for (int i = 0; i < dataSize.x; ++i) {
		for (int j = 0; j < dataSize.y; ++j) {
			auto posFunc = advectedData->dataPosition();
			auto current_pos = posFunc(i, j);
			auto current_vec = flow->sample(current_pos);
			//auto backTracedPoint = current_pos - current_vec * timeIntervalInSeconds;

			auto midPoint = current_pos - 0.5 * timeIntervalInSeconds * current_vec;
			auto midVel = flow->sample(midPoint);

			auto backTracedPoint = current_pos - timeIntervalInSeconds * midVel;

			if (backTracedPoint.x >= 0 && backTracedPoint.x <= 2 &&
				backTracedPoint.y >= 0 && backTracedPoint.y <= 2) {
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

