#ifndef COLLOCATEDVECTORGRID2_HPP
#define COLLOCATEDVECTORGRID2_HPP


#include "VectorGrid2.hpp"
#include "../Array2.hpp"
#include "ArraySampler2.hpp"
#include <iostream>


#include <vector>

template<class T>
class CollocatedVectorGrid2 : public VectorGrid2<T> {
public:


	CollocatedVectorGrid2();

	virtual ~CollocatedVectorGrid2();

	virtual Vector2I dataSize() const = 0;


	virtual Vector2D dataOrigin() const = 0;

	T& operator()(int i, int j);

	double divergenceAtDataPoint(int i, int j);

	double curlAtDataPoint(int i, int j);


	std::function<Vector2D(int, int)> dataPosition() const;

	T sample(const Vector2D& x) const;

	double divergence(const Vector2D& x) const override;

	double curl(const Vector2D& x) const override;

	//����Ӧ����ʲô���ͣ�
	std::function<T(const Vector2D&)> sampler() const override;

	Array2<T>& datas();

	void fill(const T& value);


protected:

	LinearArraySampler2Ptr<T> _linearSampler;

	//��ʱ��ʵ��
	//void swapCollocatedVectorGrid(CollocatedVectorGrid2* other);

	//��ʱ��ʵ��
	//void setCollocatedVectorGrid(const CollocatedVectorGrid2& other);

	//��ʱ��ʵ��
	//void getData(std::vector<double>* data)  override;

	//��ʱ��ʵ��
	//void setData(const std::vector<double>& data) override;
	Array2<T> _data;

	virtual void onResize(
		const Vector2I& resolution,
		const Vector2D& gridSpacing,
		const Vector2D& origin,
		const T& initialValue) final;
private:

	//��ʱ��ʵ��
	//void swap(CollocatedVectorGrid2* other);



	//�����ʱ�Ȳ�ʵ��
	//LinearArraySampler2<Vector2D, double> _linearSampler;
	std::function<T(const Vector2D&)> _sampler;



	//��ʱ��ʵ��
	//void resetSampler();
};

template<class T>
using CollocatedVectorGrid2Ptr = std::shared_ptr<CollocatedVectorGrid2<T>>;



//����ط�ʮ�ֲ����
//����˵���������Ա������˵���ڹ��캯��ִ����֮ǰ�����ǻᱻ��ʼ��ô
//��ģ�����ͺ���ͨ������һ����ô��
//template<class T>
//CollocatedVectorGrid2<T>::CollocatedVectorGrid2():
//    _linearSampler(_data, Vector2D(1.0, 1.0), Vector2D(0.0,0.0))
//{
//    //LinearArraySampler2<T> sam(_data, Vector2D(1.0, 1.0), Vector2D());
//}


template<class T>
CollocatedVectorGrid2<T>::CollocatedVectorGrid2()
{

}

template<class T>
CollocatedVectorGrid2<T>::~CollocatedVectorGrid2() {
}


template<class T>

T& CollocatedVectorGrid2<T>::operator()(int i, int j) {
	return _data(i, j);
}

template<class T>
double CollocatedVectorGrid2<T>::divergenceAtDataPoint(int i, int j) {
	return 0.0;
}


template<class T>
double CollocatedVectorGrid2<T>::curlAtDataPoint(int i, int j) {
	return 0.0;
}

template<class T>
std::function<Vector2D(int, int)> CollocatedVectorGrid2<T>::dataPosition() const {
	Vector2D dataOrigin_ = dataOrigin();
	return [this, dataOrigin_](double i, double j) -> Vector2D {
		return dataOrigin_ + this->gridSpacing() * Vector2D({ i, j });
	};
}


//ע������sampler������ʵ��
//�������ʹ�ú�����������Ż�
//���Բ���������д��ʮ�ֲ��ã��������Դջ�ʹ��
//�㷨�����֮�������Ż�����ɣ���ģ�̫����
template<class T>
T CollocatedVectorGrid2<T>::sample(const Vector2D& x) const {

	auto result = (*_linearSampler)(this->_data, x);
	return result;

}


template<class T>
double CollocatedVectorGrid2<T>::divergence(const Vector2D& x) const {
	return 0.0;
}


template<class T>
double CollocatedVectorGrid2<T>::curl(const Vector2D& x) const {
	return 0.0;
}


template<class T>
std::function<T(const Vector2D&)> CollocatedVectorGrid2<T>::sampler() const {
	return _sampler;
}

template<class T>
Array2<T>& CollocatedVectorGrid2<T>::datas() {
	return _data;
}

//��������ò����Ż���
template<class T>
void CollocatedVectorGrid2<T>::fill(const T& value) {
	auto size = dataSize();
	for (int i = 0; i < size.x; ++i) {
		for (int j = 0; j < size.y; ++j) {
			_data(i, j) = value;
		}
	}
}

template<class T>
void CollocatedVectorGrid2<T>::onResize(
	const Vector2I& resolution,
	const Vector2D& gridSpacing,
	const Vector2D& origin,
	const T& initialValue) {

	_data.reSize(dataSize().x, dataSize().y, initialValue);
	//resetSampler();


}




#endif
