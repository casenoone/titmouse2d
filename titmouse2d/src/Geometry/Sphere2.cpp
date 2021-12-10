#include "Sphere2.h"

#include "Ray2.h"

Sphere2::Sphere2():
_center(Vector2<double>()),_r(1.0)
{

}

Sphere2::~Sphere2() {

}

Sphere2::Sphere2(const Vector2<double>& center, const double& r,
	const Vector2<size_t>& resolution,
	const Vector2<double>& origin,
	double initialValue
) :
_center(center),_r(r)
{
	_data = make_shared<VertexCenteredScalarGrid2>(resolution, origin, initialValue);
}

Sphere2::Sphere2(const VertexCenteredScalarGrid2& other):
ImplicitSurface2(other)
{


}
Surface2::SurfaceQueryResult Sphere2::getClosedInformation(const Vector2<double>& otherPoint) {
	SurfaceQueryResult a;
	return a;
}

Vector2<double> Sphere2::center() {
	return _center;
}

double Sphere2::r() {
	return _r;
}


//oΪ�������
//dΪ���߷���
//cΪ���������
void Sphere2::computeSdf() {
	auto dataSize = _data->dataSize();

	for (int i = 0; i < dataSize.x; ++i) {
		for (int j = 0; j < dataSize.y; ++j) {
			auto posFunc = _data->dataPosition();
			auto o = posFunc(i, j);
			auto c = _center;
			auto v = o - c;
			
			//������ڵ�ָ��Բ�ĵ�����
			Ray2 ray(o,(c-o).getNormalize());
			auto d = ray.orientation();

			auto r = _r;

			auto dv = d.dot(v);
			auto dv2 = dv * dv;
			auto v2 = v.getLengthSquared();
			auto r2 = r * r;

			//��̾���
			auto t = -dv - sqrt(dv2 - (v2 - r2));

			//�����
			auto closestPoint = o + t * d;

			//����sdf�ķ���
			auto sdf = t;
			if (o.disSquare(c) <= r2) {
				sdf = -t;
			}

			(*_data)(i, j) = sdf;
			
		}
	}

	
}