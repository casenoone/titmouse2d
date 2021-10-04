/*
**序言
* 这是titmouse2d的第一个头文件
* 编写于2021.10.03
**以上
*/
#ifndef VECTOR2_H
#define VECTOR2_H

//复制构造函数与重载等于号有区别吗？
//答：有区别

//复制构造函数不仅仅在使用等于号时会调用
//而且还会在传参等地方调用

//注意const函数的用法

//注意inline函数在函数实现的时候也加上inline关键字

template<class T>
class Vector2 {
public:
	Vector2();
	
	~Vector2();
	
	Vector2(const T& _x, const T& _y);
	
	//拷贝构造函数
	Vector2(const Vector2<T>&);

	//用s来设置当前对象的属性
	inline void set(const T& s);


	//运算符重载
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

	//点积运算
	inline T dot(const Vector2<T>& vec) const;

	//获取向量长度
	inline T getLength()const;

	//获取向量长度的平方
	inline T getLengthSquared()const;

	//向量单位化，改变当前对象的属性
	inline Vector2<T> normalize();

	//获取单位化后的结果，但并不改变当前对象属性
	inline Vector2<T> getNormalize()const;

	//叉积运算
	inline T cross(const Vector2<T>& vec)const;

	//求两vector之间的距离，此时vector代表点
	inline T dis(const Vector2<T>& vec)const;

	//求两vector之间的距离的平方，此时vector代表点
	inline T disSquare(const Vector2<T>& vec)const;

	//求vector最小的分量
	inline T min()const;

	//返回(v,v)/this
	inline Vector2<T> rdiv(const T& v)const;
	
	//计算当前vector在normal上的投影
	inline Vector2<T> projected(const Vector2<T>& normal) const;

public:
	T x;
	T y;

};



#endif