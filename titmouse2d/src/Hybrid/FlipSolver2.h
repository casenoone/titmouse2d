#ifndef FLIPSOLVER2_H
#define FLIPSOLVER2_H

#include "PicSolver2.h"

class FlipSolver2 : public PicSolver2 {

public:
	class Builder;

	FlipSolver2();

	FlipSolver2(
		const Vector2I& resolutions,
		const Vector2D& gridSpacing,
		const Vector2D& gridOrigin);

	virtual ~FlipSolver2();


	//����pic��ռ�ı���
	double picBlendingFactor()const;

	//���ֵΪ0-1
	//Ϊ�˼���noise�������������
	void setPicBlendingFactor(double factor);

	static Builder builder();

protected:
	void transferFromParticlesToGrids() override;

	void transferFromGridsToParticles() override;

private:
	double _picBlendingFactor = 0.01;
	Array2D _uDelta;
	Array2D _vDelta;

};

typedef std::shared_ptr<FlipSolver2> FlipSolver2Ptr;


class FlipSolver2::Builder final
	: public GridFluidSolverBuilderBase2<FlipSolver2::Builder> {
public:
	FlipSolver2 build() const;

	FlipSolver2Ptr makeShared() const;
};




#endif