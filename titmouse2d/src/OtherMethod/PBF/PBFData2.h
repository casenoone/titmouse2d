#ifndef PBFDATA2_H
#define PBFDATA2_H

#include "../../Lagrangian/ParticleSystemData2.h"

class PBFData2 : public ParticleSystemData2 {
public:
	PBFData2() = default;

public:
	Array<double> densities();

	Array<double> lambda();

	Array<Vector2<double>> deltaP();

private:
	Array<double> _densities;
	Array<double> _lambda;
	Array<Vector2<double>> _deltaP;
};

using PBFData2Ptr = shared_ptr<PBFData2>;


inline Array<double> PBFData2::densities() {
	return _densities;
}

inline Array<double> PBFData2::lambda() {
	return _lambda;
}


inline Array<Vector2<double>> PBFData2::deltaP() {
	return _deltaP;
}

#endif