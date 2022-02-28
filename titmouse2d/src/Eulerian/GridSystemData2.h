#ifndef GRIDSYSTEMDATA_H
#define GRIDSYSTEMDATA_H

#include "FaceCenteredGrid2.h"
#include "ScalarGrid2.h"

#include "VertexCenteredScalarGrid2.h"

class GridSystemData2 {
public:
	GridSystemData2();

	GridSystemData2(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin);

	GridSystemData2(const GridSystemData2& other);

	virtual ~GridSystemData2();

	void resize(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin);

	Vector2I resolution() const;

	Vector2D gridSpacing() const;

	Vector2D origin() const;


	FaceCenteredGrid2Ptr& velocity();
	VertexCenteredScalarGrid2Ptr& sdf();

private:
	Vector2I _resolution;
	Vector2D _gridSpacing;
	Vector2D _origin;

	FaceCenteredGrid2Ptr _velocity;
	VertexCenteredScalarGrid2Ptr _sdf;
};

typedef std::shared_ptr<GridSystemData2> GridSystemData2Ptr;


#endif