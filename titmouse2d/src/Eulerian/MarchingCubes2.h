#ifndef MARCHINGCUBES2_H
#define MARCHINGCUBES2_H

#include "VertexCenteredScalarGrid2.h"

struct Edge {
	Vector2<double> p1;
	Vector2<double> p2;
};

struct Circle {
	double r;
	Vector2<double> center;

};

struct LineSegment {
	Vector2<double> start;
	Vector2<double> End;
};


class MarchingCube2 {
public:
	MarchingCube2();
	MarchingCube2(const Vector2<size_t>& resolution,
		const Vector2<double>& origin ,
		double initialValue = 1.0,
		double domainSizeX = 1.0);
	~MarchingCube2();

	class Builder;

	static Builder builder();

	//�����ֵ��
	void getLineSegmentSet(vector<LineSegment>& lineSet);

	void setCircleList(vector<Circle>& circleList);

public:
	//���marchingcube�㷨�ؽ�����ʱ�ı�����
	VertexCenteredScalarGrid2Ptr data;

private:
	//��������ڵ�Ȩ��
	void calculateWeight();

	//��ȡ���и��ӵ�������Ϣ
	void getVoxelConfig(Array2Ptr<double>& num);

	//�����ֵ�����ڵ�λ��
	Vector2<double> calculateIso(size_t edge, size_t i, size_t j);

	//���Բ
	vector<Circle> _circleList;


};


typedef std::shared_ptr<MarchingCube2> MarchingCube2Ptr;



class MarchingCube2::Builder {
public:

	Builder();
	
	~Builder();

	Builder& withResolution(const Vector2<size_t>& resolution);

	Builder& withResolution(size_t resolutionX, size_t resolutionY);

	Builder& withOrigin(const Vector2<double>& gridOrigin);

	Builder& withOrigin(double gridOriginX, double gridOriginY);

	Builder& withInitialValue(double initialVal);
	
	Builder& withDomainSizeX(double initialVal);

	MarchingCube2 build() const;

	MarchingCube2Ptr makeShared() const;

private:
	Vector2<size_t> _resolution;
	Vector2<double> _gridOrigin;
	double _initialVal = 0.0;
	double _domainSizeX = 1.0;

};



#endif