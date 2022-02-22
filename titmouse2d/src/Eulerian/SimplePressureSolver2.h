#ifndef SIMPLEPRESSURESOLVER2_H
#define SIMPLEPRESSURESOLVER2_H

#include <iostream>
using namespace std;

#include "FaceCenteredGrid2.h"
#include "../VectorN.hpp"
#include "../SparseMatrix.hpp"

#include "../Array2.hpp"

class SimplePressureSolver2 {
public:
	SimplePressureSolver2();

	~SimplePressureSolver2();

	void solve(FaceCenteredGrid2Ptr flow, const Array2<int>& markers);

private:
	int fluidCellSize(const Array2<int>& markers);

	void constructMatrix(FaceCenteredGrid2Ptr& flow,
		const Array2<int>& markers,
		VectorN<double>& x, int systemSize);

	void applyGradientandUpdateVel(FaceCenteredGrid2Ptr& flow,
		const Array2<int>& markers,
		VectorN<double>& x);

};

typedef std::shared_ptr<SimplePressureSolver2> SimplePressureSolver2Ptr;


#endif