#ifndef EULERIANSMOKEGRIDDATA2_H
#define EULERIANSMOKEGRIDDATA2_H

#include "../GridSystemData2.h"

class EulerianSmokeGridData2 : public GridSystemData2 {
public:
	EulerianSmokeGridData2() = default;

	EulerianSmokeGridData2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin);

	CellCenteredScalarGrid2Ptr  densities();

	CellCenteredScalarGrid2Ptr  temperature();

private:
	CellCenteredScalarGrid2Ptr _densities;

	CellCenteredScalarGrid2Ptr _temperature;
};

typedef std::shared_ptr<EulerianSmokeGridData2> EulerianSmokeGridData2Ptr;

inline EulerianSmokeGridData2::EulerianSmokeGridData2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& origin) :GridSystemData2(resolution, gridSpacing, origin) {

	_densities = std::make_shared<CellCenteredScalarGrid2>(resolution, origin, 0.0);
	_temperature = std::make_shared<CellCenteredScalarGrid2>(resolution, origin, 0.0);

}

inline CellCenteredScalarGrid2Ptr  EulerianSmokeGridData2::densities() {
	return _densities;
}

inline CellCenteredScalarGrid2Ptr  EulerianSmokeGridData2::temperature() {
	return _temperature;
}


#endif