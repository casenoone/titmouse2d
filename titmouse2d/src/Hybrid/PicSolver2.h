#ifndef PICSOLVER2_H
#define PICSOLVER2_H

#include "../Eulerian/GridFluidSolver2.h"
#include "../Lagrangian/ParticleSystemSolver2.h"

#include "../ConstVar.h"


class PicSolver2 :public GridFluidSolver2 {
public:
	class Builder;

	PicSolver2();

	PicSolver2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);

	virtual ~PicSolver2();

	void setCollider(const Collider2& _collider);

	const ParticleSystemData2Ptr& particleSystemData() const;

	//临时测试使用,后期改成Builder里构建
	void setData(int numberOfParticles, Array<Vector2D>& pos,
		int resolutionX, int resolutionY);

	void onBeginAdvanceTimeStep(double timeIntervalInSeconds) override;

	void onAdvanceTimeStep(double timeIntervalInSeconds)override;

	static Builder builder();

public:

	Collider2 collider;

protected:

	void computePressure(double timeIntervalInSeconds)override;

	void computeAdvection(double timeIntervalInSeconds) override;

	virtual void transferFromParticlesToGrids();

	virtual void transferFromGridsToParticles();

	virtual void moveParticles(double timeIntervalInSeconds);

	void setMarkers();

	void setFluidCellNum();

protected:

	ParticleSystemData2Ptr _particles;

	//这个貌似是为了外推速度场用的，但是对于pic/flip，外推速度场怎么操作呢？目的是什么？
	Array2D _uMarkers;
	Array2D _vMarkers;
};

typedef std::shared_ptr<PicSolver2> PicSolver2Ptr;


class PicSolver2::Builder final
	: public GridFluidSolverBuilderBase2<PicSolver2::Builder> {
public:
	PicSolver2 build() const;

	PicSolver2Ptr makeShared() const;
};




#endif