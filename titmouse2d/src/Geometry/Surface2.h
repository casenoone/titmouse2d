#ifndef SURFACE2_H
#define SURFACE2_H

#include <iostream>
using namespace std;

//������໮��Ϊ��ʽ�������ʽ����
//Ȼ����������ʽ����ʽ֮���ת��
class Surface2 {
public:
	Surface2();

	virtual ~Surface2();

	virtual Vector2<double> closestPoint(const Vector2<double>& otherPoint)const;

	virtual double closestDistance(const Vector2<double>& otherPoint)const;

	virtual void ghost() const{
		
	}

};

using Surface2Ptr = shared_ptr<Surface2>;


#endif