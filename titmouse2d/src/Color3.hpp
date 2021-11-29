#ifndef COLOR3_H
#define COLOR3_H


template<class T>
class Color3 {
public:
	Color3();

	Color3(const T& _r, const T& _g, const T& _b);

	~Color3();

	inline Color3<T> operator*(const T& r) const;

	template<typename T>
	friend Color3<T> operator* (const T& r, const Color3<T>& obj);

public:
	T r = static_cast<T>(0);
	T g = static_cast<T>(0);
	T b = static_cast<T>(0);
};


template<class T>
Color3<T>::Color3() {

}

template<class T>
Color3<T>::Color3(const T& _r, const T& _g, const T& _b):
r(_r),g(_g),b(_b){

}


template<class T>
Color3<T>::~Color3() {

}

template<class T>
inline Color3<T> Color3<T>::operator*(const T& r) const {
	Color3 color(this->r * r, this->b * r, this->g * r);
	return color;
}

template<typename T>
Color3<T> operator* (const T& r, const Color3<T>& obj) {
	return obj * r;
}

#endif