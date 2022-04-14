#ifndef SPHSYSTEMDATA2_H
#define SPHSYSTEMDATA2_H

#include "ParticleSystemData2.h"
#include <vector>

#include "../Vector2.hpp"
#include "../Array2.hpp"
#include "../Lagrangian/SphStdKernel2.h"
#include "../Lagrangian/SphSpikyKernel2.h"



class SphSystemData2 : public ParticleSystemData2 {
public:
	SphSystemData2();
	virtual ~SphSystemData2();
	ArrayD densities();
	ArrayD pressures();

	//初始化粒子密度场
	void initDensity();
	//更新密度场
	void updateDensities();
	void clearDensities();


	Vector2D gradientAt(int i, ArrayD& values);
	double laplacianAt(int i, const ArrayD& values);
private:
	ArrayD _densities;
	ArrayD _pressures;
};
using SphSystemData2Ptr = std::shared_ptr<SphSystemData2>;




#endif