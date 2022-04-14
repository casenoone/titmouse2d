#include "RegularPolygon.h"

#include "../ConstVar.h"

RegularPolygon::RegularPolygon(int edgeNum_, Vector2D center_, double r_) :
	_edgeNum(edgeNum_),
	_center(center_),
	_r(r_) {

	_data.resize(edgeNum_);

	double ca = 0;
	auto aiv = 360.0 / edgeNum_;
	auto ata = kPiD / 180;
	std::vector<Vector2D> list(edgeNum_);
	for (int k = 0; k < edgeNum_; k++) {
		auto x = cos(ca * ata) * r_ + center_.x;
		auto y = sin(ca * ata) * r_ + center_.y;
		Vector2D temp(x, y);
		list[k] = temp;
		ca += aiv;
	}
	int i = 0;
	SurfaceElement2 temp1;
	for (; i < edgeNum_ - 1; ++i) {
		temp1.start = list[i];
		temp1.end = list[i + 1];
		auto mid = (temp1.start + temp1.end) * 0.5;
		temp1.normal = (mid - center_).getNormalize();
		_data[i] = temp1;
	}

	temp1.start = list[0];
	temp1.end = list[i];
	auto mid = (temp1.start + temp1.end) * 0.5;
	temp1.normal = (mid - center_).getNormalize();
	_data[i] = temp1;

}