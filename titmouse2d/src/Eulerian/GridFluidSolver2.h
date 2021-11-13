#ifndef GRIDFLUIDSOLVER2_H
#define GRIDFLUIDSOLVER2_H

#include "../Vector2.hpp"

class GridFluidSolver2 {
public:
	class Builder;

	GridFluidSolver2();

	~GridFluidSolver2();

	GridFluidSolver2(const Vector2<size_t>& resolution, const Vector2<double>& gridOrigin);


protected:

};



#endif