#ifndef PBFDATA2_H
#define PBFDATA2_H

#include "../../Lagrangian/ParticleSystemData2.h"

class PBFData2 : public ParticleSystemData2 {
public:
	PBFData2() = default;

public:
	ArrayD densities();

	ArrayD lambda();

	Array<Vector2D> deltaP();

private:
	ArrayD _densities;
	ArrayD _lambda;
	Array<Vector2D> _deltaP;
};

using PBFData2Ptr = std::shared_ptr<PBFData2>;


inline ArrayD PBFData2::densities() {
	return _densities;
}

inline ArrayD PBFData2::lambda() {
	return _lambda;
}


inline Array<Vector2D> PBFData2::deltaP() {
	return _deltaP;
}

#endif