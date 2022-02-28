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

	static Builder builder();

private:

	void advection(double timeIntervalInSeconds);

	void pressureSolve(double timeIntervalInSeconds);

	void couplingSolve(double timeIntervalInSeconds);

	ShallowWaveData2Ptr _shallowWaveData;

};

typedef std::shared_ptr<ShallowWaveSolver2> ShallowWaveSolver2Ptr;

inline ShallowWaveSolver2::ShallowWaveSolver2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) : GridFluidSolver2(resolution, gridSpacing, gridOrigin) {

	_grids = make_shared<ShallowWaveData2>(resolution, gridSpacing, gridOrigin);
	_shallowWaveData = make_shared<ShallowWaveData2>(resolution, gridSpacing, gridOrigin);
	_shallowWaveData = std::dynamic_pointer_cast<ShallowWaveData2>(_grids);
}


inline ShallowWaveSolver2::~ShallowWaveSolver2() {

}

inline ShallowWaveData2Ptr ShallowWaveSolver2::shallowWaveData() {
	return _shallowWaveData;
}

#endif
