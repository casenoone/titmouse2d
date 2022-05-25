#include "Heart2.h"

Heart2::Heart2(const Vector2D& center, const double& r,
	const Vector2I& resolution,
	const Vector2D& origin,
	double initialValue) {

	_data = std::make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);

	computeSdf();

}

Surface2::SurfaceQueryResult Heart2::getClosedInformation(const Vector2D& otherPoint) {
	Surface2::SurfaceQueryResult result;

	return result;
}


const VertexCenteredScalarGrid2Ptr Heart2::sdf() const {

	return _data;
}


void Heart2::computeSdf() {
	auto dataSize = _data->dataSize();
	for (int i = 0; i < dataSize.x; ++i) {
		for (int j = 0; j < dataSize.y; ++j) {
			auto posFunc = _data->dataPosition();
			auto p = posFunc(i, j);

			p.x -= 0.9;
			p.x = fabs(p.x);
			p.y -= 0.8;

			auto p1 = (p - Vector2D(0.25, 0.75));
			if ((p.y + p.x) > 1.0) {
				(*_data)(i, j) = std::sqrt(p1.dot(p1)) - sqrt(2.0) / 4.0;
			}
			else {

				auto p2 = p - Vector2D(0.0, 1.0);
				auto p3 = Vector2D(p.x - 0.5 * std::max(p.x + p.y, 0.0), p.y - 0.5 * std::max(p.x + p.y, 0.0));
				(*_data)(i, j) = sqrt(std::min(p2.dot(p2), p3.dot(p3))) * sign(p.x - p.y);
			}
			(*_data)(i, j) += 0.1;

		}
	}

}