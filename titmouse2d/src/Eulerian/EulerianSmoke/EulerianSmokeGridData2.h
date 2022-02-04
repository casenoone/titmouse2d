#ifndef EULERIANSMOKEGRIDDATA2_H
#define EULERIANSMOKEGRIDDATA2_H

#include "../GridSystemData2.h"

class EulerianSmokeGridData2 : public GridSystemData2 {
public:
	EulerianSmokeGridData2() = default;

	EulerianSmokeGridData2(
		const Vector2<size_t>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& origin);

	const CellCenteredScalarGrid2Ptr const densities()const;

	const CellCenteredScalarGrid2Ptr const temperature()const;

private:
	CellCenteredScalarGrid2Ptr _densities;

	CellCenteredScalarGrid2Ptr _temperature;
};

typedef std::shared_ptr<EulerianSmokeGridData2> EulerianSmokeGridData2Ptr;

EulerianSmokeGridData2::EulerianSmokeGridData2(
	const Vector2<size_t>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& origin) :GridSystemData2(resolution, gridSpacing, origin) {

	//对密度和温度初始化

}

const CellCenteredScalarGrid2Ptr const EulerianSmokeGridData2::densities()const {
	return _densities;
}

const CellCenteredScalarGrid2Ptr const EulerianSmokeGridData2::temperature()const {
	return _temperature;
}


#endif