/*
**����
* ����titmouse2d�ĵ�һ��ͷ�ļ�
* ��д��2021.10.03
**����
*/
#ifndef VECTOR2_H
#define VECTOR2_H

//���ƹ��캯�������ص��ں���������
//��������

//���ƹ��캯����������ʹ�õ��ں�ʱ�����
//���һ����ڴ��εȵط�����

//ע��const�������÷�

//ע��inline�����ں���ʵ�ֵ�ʱ��Ҳ����inline�ؼ���

template<class T>
class Vector2 {
public:
	Vector2();
	
	~Vector2();
	
	Vector2(const T& _x, const T& _y);
	
	//�������캯��
	Vector2(const Vector2<T>&);

	//��s�����õ�ǰ���������
	inline void set(const T& s);


	//���������
	inline Vector2<T> operator+(const Vector2<T>& vec) const;

	inline Vector2<T> operator-(const Vector2<T>& vec) const;

	inline Vector2<T> operator*(const T& r) const;

	inline Vector2<T> operator*(const Vector2<T>& r) const;

	inline Vector2<T> operator/(const T& r) const;

	inline Vector2<T> operator/(const Vector2<T>& r) const;\

	inline Vector2<T> operator=(const Vector2<T>& vec);

	inline bool operator!=(const Vector2<T>& r)const;

	inline bool operator==(const Vector2<T>& r)const;

	inline Vector2<T> operator+=(const Vector2<T>& vec) const;

	inline Vector2<T> operator-=(const Vector2<T>& vec) const;

	inline Vector2<T> operator*=(const T& r) ;

	inline Vector2<T> operator*=(const Vector2<T>& r);

	inline Vector2<T> operator/=(const T& r);

	inline Vector2<T> operator/=(const Vector2<T>& r);

	//�������
	inline T dot(const Vector2<T>& vec) const;

	//��ȡ��������
	inline T getLength()const;

	//��ȡ�������ȵ�ƽ��
	inline T getLengthSquared()const;

	//������λ�����ı䵱ǰ���������
	inline Vector2<T> normalize();

	//��ȡ��λ����Ľ�����������ı䵱ǰ��������
	inline Vector2<T> getNormalize()const;

	//�������
	inline T cross(const Vector2<T>& vec)const;

	//����vector֮��ľ��룬��ʱvector�����
	inline T dis(const Vector2<T>& vec)const;

	//����vector֮��ľ����ƽ������ʱvector�����
	inline T disSquare(const Vector2<T>& vec)const;

	//��vector��С�ķ���
	inline T min()const;

	//����(v,v)/this
	inline Vector2<T> rdiv(const T& v)const;
	
	//���㵱ǰvector��normal�ϵ�ͶӰ
	inline Vector2<T> projected(const Vector2<T>& normal) const;

public:
	T x;
	T y;

};



#endif