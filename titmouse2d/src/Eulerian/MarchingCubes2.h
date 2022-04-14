#ifndef MARCHINGCUBES2_H
#define MARCHINGCUBES2_H

#include "VertexCenteredScalarGrid2.h"

#include "../Geometry/ExplicitSurface2.h"

struct Edge {
	Vector2D p1;
	Vector2D p2;
};

struct Circle {
	double r;
	Vector2D center;

};

struct LineSegment {
	Vector2D start;
	Vector2D End;
};


class MarchingCube2 {
public:
	MarchingCube2();
	MarchingCube2(const Vector2I& resolution,
		const Vector2D& origin,
		double initialValue = 1.0,
		double domainSizeX = 1.0);
	~MarchingCube2();

	class Builder;

	static Builder builder();

	//计算等值线
	void getLineSegmentSet(std::vector<LineSegment>& lineSet);

	void setCircleList(std::vector<Circle>& circleList);

	void setScalarField(const VertexCenteredScalarGrid2Ptr& _data);

	void getLineSegmentSet(std::vector<SurfaceElement2>& SurfaceSet,
		const VertexCenteredScalarGrid2Ptr& _data);

	void getLineSegmentSet(std::vector<LineSegment>& lineSet,
		const VertexCenteredScalarGrid2Ptr& _data);

public:
	//存放marchingcube算法重建表面时的标量场
	VertexCenteredScalarGrid2Ptr data;

private:
	//计算网格节点权重
	void calculateWeight();

	//获取所有格子的配置信息
	void getVoxelConfig(Array2D& num);

	//计算等值线所在的位置
	Vector2D calculateIso(int edge, int i, int j);

	//存放圆
	std::vector<Circle> _circleList;


};


typedef std::shared_ptr<MarchingCube2> MarchingCube2Ptr;



class MarchingCube2::Builder {
public:

	Builder();

	~Builder();

	Builder& withResolution(const Vector2I& resolution);

	Builder& withResolution(int resolutionX, int resolutionY);

	Builder& withOrigin(const Vector2D& gridOrigin);

	Builder& withOrigin(double gridOriginX, double gridOriginY);

	Builder& withInitialValue(double initialVal);

	Builder& withDomainSizeX(double initialVal);

	MarchingCube2 build() const;

	MarchingCube2Ptr makeShared() const;

private:
	Vector2I _resolution;
	Vector2D _gridOrigin;
	double _initialVal = 0.0;
	double _domainSizeX = 1.0;

};



#endif