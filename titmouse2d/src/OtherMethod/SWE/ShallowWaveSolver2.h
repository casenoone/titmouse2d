#ifndef SHALLOWWAVESOLVER2_H
#define SHALLOWWAVESOLVER2_H

#include "../../Eulerian/GridFluidSolver2.h"
#include "ShallowWaveData2.h"

class ShallowWaveSolver2 : public GridFluidSolver2 {
public:
	class Builder;

	ShallowWaveSolver2() = default;

	ShallowWaveSolver2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);

	virtual ~ShallowWaveSolver2();

	ShallowWaveData2Ptr shallowWaveData();

	void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	//标记哪些格子被固体所占据
	//我们暂时就假设障碍物是一个长方体
	void setMarkers(const Vector2D& lower, const Vector2D& upper);

	//设置一个圆形的移动边界
	void setSphereMarkers(const Vector2D& center, const double r);

	//返回水面数据以供OpenGL渲染
	int getWaterSurface(float mesh[]);

	static Builder builder();

private:

	void setCouplingCellNum();

	void pressureSolve(double timeIntervalInSeconds);

	void couplingSolve(double timeIntervalInSeconds);

	void applyGhostVolumn(double timeIntervalInSeconds);

	ShallowWaveData2Ptr _shallowWaveData;

};

typedef std::shared_ptr<ShallowWaveSolver2> ShallowWaveSolver2Ptr;

inline ShallowWaveSolver2::ShallowWaveSolver2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) : GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

	_grids = std::make_shared<ShallowWaveData2>(resolution, gridSpacing, gridOrigin);
	_shallowWaveData = std::make_shared<ShallowWaveData2>(resolution, gridSpacing, gridOrigin);
	_shallowWaveData = std::dynamic_pointer_cast<ShallowWaveData2>(_grids);
}


inline ShallowWaveSolver2::~ShallowWaveSolver2() {

}

inline ShallowWaveData2Ptr ShallowWaveSolver2::shallowWaveData() {
	return _shallowWaveData;
}




class ShallowWaveSolver2::Builder final
	: public GridFluidSolverBuilderBase2<ShallowWaveSolver2::Builder> {
public:
	ShallowWaveSolver2 build() const;

	ShallowWaveSolver2Ptr makeShared() const;
};



inline ShallowWaveSolver2::Builder ShallowWaveSolver2::builder() {
	return Builder();
}

inline ShallowWaveSolver2 ShallowWaveSolver2::Builder::build() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return ShallowWaveSolver2(_resolution, gridSpacing, _gridOrigin);
}


inline ShallowWaveSolver2Ptr ShallowWaveSolver2::Builder::makeShared() const {
	auto gridSpacing = Vector2D(2.0 / _resolution.x, 2.0 / _resolution.y);
	return std::shared_ptr<ShallowWaveSolver2>(
		new ShallowWaveSolver2(
			_resolution,
			gridSpacing,
			_gridOrigin),
		[](ShallowWaveSolver2* obj) {
			delete obj;
		});
}

#endif
