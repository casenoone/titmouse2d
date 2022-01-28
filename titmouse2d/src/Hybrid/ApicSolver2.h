#ifndef APICSOLVER2_H
#define APICSOLVER2_H

#include "PicSolver2.h"

//注意APIC在MAC网格上的问题

class ApicSolver2 : public PicSolver2 {
public:

	class Builder;

	ApicSolver2() = default;

	ApicSolver2(
		const Vector2<size_t>& resolutions,
		const Vector2<double>& gridSpacing,
		const Vector2<double>& gridOrigin);

	virtual ~ApicSolver2();

	void onAdvanceTimeStep(double timeIntervalInSeconds);

	static Builder builder();

protected:
	void transferFromParticlesToGrids() override;

	void transferFromGridsToParticles() override;

private:
	ArrayPtr<Vector2<double>> _cX;
	ArrayPtr<Vector2<double>> _cY;
};


typedef std::shared_ptr<ApicSolver2> ApicSolver2Ptr;


class ApicSolver2::Builder final
	: public GridFluidSolverBuilderBase2<ApicSolver2::Builder> {
public:
	ApicSolver2 build() const;

	ApicSolver2Ptr makeShared() const;
};




#endif