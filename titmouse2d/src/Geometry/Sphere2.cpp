#include "Sphere2.h"

#include "Ray2.h"

Sphere2::Sphere2() :
	_center(Vector2D()), _r(1.0)
{

}

Sphere2::~Sphere2() {

}

Sphere2::Sphere2(const Vector2D& center, const double& r,
	const Vector2I& resolution,
	const Vector2D& origin,
	double initialValue
) :
	_center(center), _r(r), ImplicitSurface2(resolution, origin, initialValue)
{
	//_data = make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);

	//这个方法应该单独拎出来，把resolution传给computeSdf这个方法
	computeSdf();
	computeBoundingBox();
}

Sphere2::Sphere2(const VertexCenteredScalarGrid2& other) :
	ImplicitSurface2(other)
{


}


//注意这个函数的意义
//一般我们使用隐式曲面，有两种情形：
//1.需要一个sdf，那么这个函数就没什么用处，
//2.有些曲面比如圆、椭圆等，在与粒子作碰撞检测时就无需sdf，直接通过联立公式进行判断，这时就需要用到此方法
Surface2::SurfaceQueryResult Sphere2::getClosedInformation(const Vector2D& otherPoint) {
	Surface2::SurfaceQueryResult result;





	//result.distance = ;
	//result.normal = ;
	//result.point = ;
	//result.velocity = ;

	return result;
}


const VertexCenteredScalarGrid2Ptr Sphere2::sdf() const {
	return _data;
}


Vector2D Sphere2::center() {
	return _center;
}

double Sphere2::r() {
	return _r;
}


//o为射线起点
//d为射线方向
//c为球体的球心
void Sphere2::computeSdf() {
	auto dataSize = _data->dataSize();
	for (int i = 0; i < dataSize.x; ++i) {
		for (int j = 0; j < dataSize.y; ++j) {
			auto posFunc = _data->dataPosition();
			auto o = posFunc(i, j);
			auto c = _center;
			auto v = o - c;

			//由网格节点指向圆心的射线
			Ray2 ray(o, (c - o).getNormalize());
			auto d = ray.orientation();
			auto r = _r;

			auto dv = d.dot(v);
			auto dv2 = dv * dv;
			auto v2 = v.getLengthSquared();

			auto r2 = r * r;

			//最短距离
			auto t = -dv - sqrt(dv2 - (v2 - r2));


			//最近点
			auto closestPoint = o + t * d;

			//计算sdf的符号
			auto sdf = t;
			if (o.disSquare(c) <= r2) {
				//sdf = -t;
			}

			(*_data)(i, j) = sdf;

		}
	}


}