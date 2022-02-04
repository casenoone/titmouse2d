#ifndef SPHSYSTEMDATA2_H
#define SPHSYSTEMDATA2_H

#include "ParticleSystemData2.h"
#include <vector>
using namespace std;

#include "../Vector2.hpp"
#include "../Array2Ptr.hpp"
#include "../Lagrangian/SphStdKernel2.h"
#include "../Lagrangian/SphSpikyKernel2.h"



class SphSystemData2 : public ParticleSystemData2 {
public:
	SphSystemData2();
	virtual ~SphSystemData2();
	ArrayPtr<double> densities();
	ArrayPtr<double> pressures();

	//初始化粒子密度场
	void initDensity();
	//更新密度场
	void updateDensities();
	void clearDensities();


	Vector2<double> gradientAt(size_t i, ArrayPtr<double>& values);
	double laplacianAt(size_t i, const ArrayPtr<double>& values);
private:
	ArrayPtr<double> _densities;
	ArrayPtr<double> _pressures;
};
using SphSystemData2Ptr = shared_ptr<SphSystemData2>;




#endif