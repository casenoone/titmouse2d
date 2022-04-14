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
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);
public:

	//网格属性
	CellCenteredVectorGrid2Ptr<Vector2D> g_velocity;
	CellCenteredScalarGrid2Ptr g_mass;

	//粒子属性
	ArrayD J;
	Array<Matrix2x2<double>> C;

};

using MPMData2Ptr = std::shared_ptr<MPMData2>;

inline MPMData2::MPMData2(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& gridOrigin) {

	g_velocity = std::make_shared<CellCenteredVectorGrid2<Vector2D>>(resolution, gridOrigin, Vector2D());
	g_mass = std::make_shared<CellCenteredScalarGrid2>(resolution, gridOrigin, 0.0);

}

#endif