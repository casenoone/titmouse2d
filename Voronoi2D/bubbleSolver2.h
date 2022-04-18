#pragma once

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "bubbleData2.h"
#include "../Voronoi2D/Voronoi2.h"
#include "../titmouse2d/src/boundingbox2.h"
#include <Eigen/Dense>


//调节气泡的干湿状态
static const double lambda = 0.0;

static const double v_eps = 0.08;
class BubbleSolver2 : public ParticleSystemSolver2 {
public:
	BubbleSolver2();

	virtual ~BubbleSolver2() {}

	void emitParticles();

	void timeIntegration(double timeIntervalInSeconds, const Voronoi2& voronoi);

	void onAdvanceTimeStep(double timeIntervalInSeconds, const Voronoi2& voronoi);

	double computeRestLen(double r_i, double r_j);

	void computeStrongForce(const Voronoi2& voronoi);

	void computeDragForce(const Voronoi2& voronoi);

	void computeDragForce(const Voronoi2& voronoi, int i);

	void computeRepulsiveForce();

	Vector2D computeSingeStrongForce(const Vector2D& p1, const Vector2D& p2, double l_ij);

	//以下是动力学求解部分

	Vector2D computeUSingle(const Vector2D& pos, int i)const;

	void emitVortexRing();


public:
	BubbleDataPtr _bubbleData;

private:


};


inline double BubbleSolver2::computeRestLen(double r_i, double r_j) {
	return r_i + r_j;
	return std::sqrt(r_i * r_i + r_j * r_j + (3 * lambda - 1) * r_i * r_j);

}


inline Vector2D BubbleSolver2::computeUSingle(const Vector2D& pos, int i)const {

	auto& position = _bubbleData->vortexPosition;
	auto vorticity = _bubbleData->vorticities();
	auto r2 = (pos - position[i]).getLengthSquared();
	auto uv = Vector2D(position[i].y - pos.y, pos.x - position[i].x);
	return vorticity[i] * uv / (kPiD * r2) * 0.5 * (1.0 - pow(kE, -r2 / (v_eps * v_eps)));

}

