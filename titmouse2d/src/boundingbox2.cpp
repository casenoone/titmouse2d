#include "boundingbox2.h"


BoundingBox2::BoundingBox2() {

}

BoundingBox2::BoundingBox2(const Vector2D& point1, const Vector2D& point2) {

	lowerCorner.x = std::min(point1.x, point2.x);
	lowerCorner.y = std::min(point1.y, point2.y);
	upperCorner.x = std::max(point1.x, point2.x);
	upperCorner.y = std::max(point1.y, point2.y);
}


BoundingBox2::~BoundingBox2() {

}