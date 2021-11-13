#ifndef GRIDSYSTEMDATA_H
#define GRIDSYSTEMDATA_H

#include "FaceCenteredGrid2.h"
#include "ScalarGrid2.h"

class GridSystemData2 {
public:
    GridSystemData2();

    GridSystemData2(
        const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin);

    GridSystemData2(const GridSystemData2& other);

    virtual ~GridSystemData2();

    void resize(
        const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin);

    Vector2<size_t> resolution() const;

    Vector2<double> gridSpacing() const;

    Vector2<double> origin() const;


    FaceCenteredGrid2Ptr& velocity();

private:
    Vector2<size_t> _resolution;
    Vector2<double> _gridSpacing;
    Vector2<double> _origin;

    FaceCenteredGrid2Ptr _velocity;

};

typedef std::shared_ptr<GridSystemData2> GridSystemData2Ptr;


#endif