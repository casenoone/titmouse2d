#ifndef SHALLOWWAVEDATA2_H
#define SHALLOWWAVEDATA2_H

#include "../../random.h"

class ShallowWaveData2 : public GridSystemData2 {
public:

	ShallowWaveData2() = default;

	ShallowWaveData2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin);

public:
	CellCenteredScalarGrid2Ptr height;

	CellCenteredScalarGrid2Ptr old_height;

	Array2I markers;

	Array2D ghostH;

	Array2D solveSystemMarker;



};

typedef std::shared_ptr<ShallowWaveData2> ShallowWaveData2Ptr;


inline ShallowWaveData2::ShallowWaveData2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& origin) :GridSystemData2(resolution, gridSpacing, origin) {

	//默认初始化水面高度为initHeight
	double initHeight = 0.5;
	height = std::make_shared<CellCenteredScalarGrid2>(resolution, Vector2D(), initHeight);
	old_height = std::make_shared<CellCenteredScalarGrid2>(resolution, Vector2D(), initHeight);

	solveSystemMarker.reSize(resolution.x, resolution.y, -1);
	markers.reSize(resolution.x, resolution.y, 0);
	ghostH.reSize(resolution.x, resolution.y, 0);
}

#endif