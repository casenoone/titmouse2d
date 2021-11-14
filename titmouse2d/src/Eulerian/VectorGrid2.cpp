#include "VectorGrid2.h"

VectorGrid2::VectorGrid2() {

}
VectorGrid2::~VectorGrid2() {

}


void VectorGrid2::clear() {
    resize(Vector2<size_t>(), gridSpacing(), origin(), Vector2<double>());
}


void VectorGrid2::resize(
    const Vector2<size_t>& resolution,
    const Vector2<double>& gridSpacing,
    const Vector2<double>& origin,
    const Vector2<double>& initialValue) {
    
    //这里一会再说
    
    //setSizeParameters(resolution, gridSpacing, origin);

    _resolution = resolution;
    _gridSpacing = gridSpacing;
    _origin = origin;

    onResize(resolution, gridSpacing, origin, initialValue);
}

void VectorGrid2::fill(const Vector2<double>& value) {

}


VectorGridBuilder2::VectorGridBuilder2() {
}

VectorGridBuilder2::~VectorGridBuilder2() {
}


