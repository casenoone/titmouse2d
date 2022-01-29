#ifndef MPMDATA2_H
#define MPMDATA2_H

#include "../../Eulerian/CellCenteredScalarGrid2.h"
#include "../../Eulerian/CellCenteredVectorGrid2.hpp"
#include "../../Lagrangian/ParticleSystemData2.h"
#include "../../Matrix2x2.hpp"
class MPMData2 : public ParticleSystemData2 {
public:
	MPMData2() = default;

public:

	//网格属性
	CellCenteredVectorGrid2Ptr<Vector2<double>> g_velocity;
	CellCenteredScalarGrid2Ptr g_mass;

	//粒子属性
	ArrayPtr<double> J;
	ArrayPtr<Matrix2x2<double>> C;

};

using MPMData2Ptr = shared_ptr<MPMData2>;


#endif