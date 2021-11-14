#ifndef PICSOLVER2_H
#define PICSOLVER2_H

#include "../Eulerian/GridFluidSolver2.h"
#include "../Lagrangian/ParticleSystemSolver2.h"

class PicSolver2 :public GridFluidSolver2 {
public:
	class Builder;

	PicSolver2();

	PicSolver2(
		const Vector2<size_t>& resolution,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);

	virtual ~PicSolver2();

	const ParticleSystemData2Ptr& particleSystemData() const;

	static Builder builder();


protected:
	void onBeginAdvanceTimeStep(double timeIntervalInSeconds) override;

	//暂时不实现
	//void computeViscosity(double timeIntervalInSeconds)override;

	void computePressure(double timeIntervalInSeconds)override;

	void computeAdvection(double timeIntervalInSeconds) override;

	virtual void transferFromParticlesToGrids();

	virtual void transferFromGridsToParticles();

	virtual void moveParticles(double timeIntervalInSeconds);



protected:

	ParticleSystemData2Ptr _particles;

	Array2Ptr<double>_uMarkers;
	Array2Ptr<double> _vMarkers;
};

typedef std::shared_ptr<PicSolver2> PicSolver2Ptr;


class PicSolver2::Builder final
	: public GridFluidSolverBuilderBase2<PicSolver2::Builder> {
public:
	PicSolver2 build() const;

	PicSolver2Ptr makeShared() const;
};




#endif