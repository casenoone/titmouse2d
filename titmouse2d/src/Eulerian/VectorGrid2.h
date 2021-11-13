#ifndef VECTORGRID2_H
#define VECTORGRID2_H

#include "Grid2.h"
#include "VectorField2.h"


class VectorGrid2 :public VectorField2, public Grid2 {
public:
    VectorGrid2();
    virtual ~VectorGrid2();

    void clear();

    void resize(const Vector2<size_t>& resolution,
        const Vector2<double>& gridSpacing,
        const Vector2<double>& origin ,
        const Vector2<double>& initialValue);
    virtual std::shared_ptr<VectorGrid2> clone() const = 0;

    virtual void fill(const Vector2<double>& value) = 0;

protected:
    virtual void onResize(const Vector2<size_t>& resolution, const Vector2<double>& gridSpacing,
        const Vector2<double>& origin,
        const Vector2<double>& initialValue) = 0;
};

typedef std::shared_ptr<VectorGrid2> VectorGrid2Ptr;

class VectorGridBuilder2 {
public:
    VectorGridBuilder2();

    virtual ~VectorGridBuilder2();

    virtual VectorGrid2Ptr build(const Vector2<size_t>& resolution,
        const Vector2<double>& gridOrigin,
        const Vector2<double>& initialVal) const = 0;
};

typedef std::shared_ptr<VectorGridBuilder2> VectorGridBuilder2Ptr;




#endif