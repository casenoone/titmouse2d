#include "AdvectionSolver2.h"

AdvectionSolver2::AdvectionSolver2() {

}

AdvectionSolver2::~AdvectionSolver2() {

}

//速度场的advection
void AdvectionSolver2::solve(const FaceCenteredGrid2Ptr& flow,
	FaceCenteredGrid2Ptr advectedData,
	double timeIntervalInSeconds) {


	auto sizeU = advectedData->uSize();
	auto sizeV = advectedData->vSize();
	auto u = advectedData->uDatas();
	auto v = advectedData->vDatas();

	Array2D newU, newV;
	newU.reSize(sizeU.x, sizeU.y);
	newV.reSize(sizeV.x, sizeV.y);

	for (int i = 0; i < sizeU.x; ++i) {
		for (int j = 0; j < sizeU.y; ++j) {
			auto posFunc = advectedData->uPosition();
			auto current_pos = posFunc(i, j);
			auto current_vec = flow->sample(current_pos);
			auto midPoint = current_pos - 0.5 * timeIntervalInSeconds * current_vec;
			auto midVel = flow->sample(midPoint);

			auto backTracedPoint = current_pos - timeIntervalInSeconds * midVel;

			if (backTracedPoint.x >= 0 && backTracedPoint.x <= 2 &&
				backTracedPoint.y >= 0 && backTracedPoint.y <= 2) {
				newU(i, j) = advectedData->sample(backTracedPoint).x;

			}
			else {

				double x, y;
				x = clamp<double>(backTracedPoint.x, 0.0, 2.0);
				y = clamp<double>(backTracedPoint.y, 0.0, 2.0);
				newU(i, j) = advectedData->sample(Vector2D(x, y)).x;
				//cout << newU(i, j) << endl;
			}
		}
	}



	for (int i = 0; i < sizeV.x; ++i) {
		for (int j = 0; j < sizeV.y; ++j) {
			auto posFunc = advectedData->vPosition();
			auto current_pos = posFunc(i, j);
			auto current_vec = flow->sample(current_pos);
			auto midPoint = current_pos - 0.5 * timeIntervalInSeconds * current_vec;
			auto midVel = flow->sample(midPoint);

			auto backTracedPoint = current_pos - timeIntervalInSeconds * midVel;

			if (backTracedPoint.x >= 0 && backTracedPoint.x <= 2 &&
				backTracedPoint.y >= 0 && backTracedPoint.y <= 2) {
				newV(i, j) = advectedData->sample(backTracedPoint).y;

			}
			else {

				double x, y;
				x = clamp<double>(backTracedPoint.x, 0.0, 2.0);
				y = clamp<double>(backTracedPoint.y, 0.0, 2.0);
				newV(i, j) = advectedData->sample(Vector2D(x, y)).y;
			}
		}
	}

	u = newU;
	v = newV;
}


//这里可以改写成多态，因为CellCentered和VertexCentered都派生自scalargrid
//实现完再改
void AdvectionSolver2::solve(const FaceCenteredGrid2Ptr& flow,
	CellCenteredScalarGrid2Ptr advectedData,
	double timeIntervalInSeconds) {
	auto dataSize = advectedData->dataSize();
	auto data = advectedData->datas();

	Array2D newData;
	//newData = data;
	newData.reSize(dataSize.x, dataSize.y, 0.0);

	for (int i = 0; i < dataSize.x; ++i) {
		for (int j = 0; j < dataSize.y; ++j) {
			auto posFunc = advectedData->dataPosition();
			auto current_pos = posFunc(i, j);
			auto current_vec = flow->sample(current_pos);

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
				newData(i, j) = advectedData->sample(Vector2D(x, y));
			}


		}
	}

	data = newData;
}


void AdvectionSolver2::solve(const FaceCenteredGrid2Ptr& flow,
	VertexCenteredScalarGrid2Ptr advectedData,
	double timeIntervalInSeconds) {

	auto dataSize = advectedData->dataSize();
	auto data = advectedData->datas();

	Array2D newData;
	//newData = data;
	newData.reSize(dataSize.x, dataSize.y, 0.0);

	for (int i = 0; i < dataSize.x; ++i) {
		for (int j = 0; j < dataSize.y; ++j) {
			auto posFunc = advectedData->dataPosition();
			auto current_pos = posFunc(i, j);
			auto current_vec = flow->sample(current_pos);

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
				newData(i, j) = advectedData->sample(Vector2D(x, y));
			}


		}
	}

	data = newData;

}

