#ifndef MPMDATA2_H
#define MPMDATA2_H

#include "../../Eulerian/CellCenteredScalarGrid2.h"
#include "../../Eulerian/CellCenteredVectorGrid2.hpp"
#include "../../Lagrangian/ParticleSystemData2.h"
#include "../../Matrix2x2.hpp"
class MPMData2 : public ParticleSystemData2 {
public:
	MPMData2() = default;

	MPMData2(
		const Vector2<int>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);
public:

	//网格属性
	CellCenteredVectorGrid2Ptr<Vector2<double>> g_velocity;
	CellCenteredScalarGrid2Ptr g_mass;

	//粒子属性
	Array<double> J;
	Array<Matrix2x2<double>> C;

};

using MPMData2Ptr = shared_ptr<MPMData2>;

inline MPMData2::MPMData2(
	const Vector2<int>& resolution,
	const Vector2<double>& gridSpacing,
	const Vector2<double>& gridOrigin) {

	g_velocity = make_shared<CellCenteredVectorGrid2<Vector2D>>(resolution, gridOrigin, Vector2D());
	g_mass = make_shared<CellCenteredScalarGrid2>(resolution, gridOrigin, 0.0);

}

#endif