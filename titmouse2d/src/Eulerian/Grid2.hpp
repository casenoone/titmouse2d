#ifndef GRID2_HPP
#define GRID2_HPP

#include <iostream>
using namespace std;

#include "../Vector2.hpp"


template<class T>
class Grid2 {
public:
	
	template<class T>
	using Grid2Ptr = shared_ptr<Grid2<T>>;
	
	Grid2();
	virtual ~Grid2();

	//下标转为位置
	typedef function<Vector2<T>(size_t, size_t)> DataPositionFunc;

	const Vector2<T>& resolution()const;
	
	const Vector2<T>& origin()const;
	
	const Vector2<T>& gridSpacing()const;

	DataPositionFunc cellCenterPosition() const;

	template<typename Callback>
	void forEachCellIndex(Callback& func) const;


protected:


private:
	Vector2<T> _resolution;
	Vector2<T> _gridSpacing;
	Vector2<T> _origin;
};

template<class T>
using Grid2Ptr = shared_ptr<Grid2<T>>;


template<class  T>
Grid2<T>::Grid2() {

}

template<class  T>
Grid2<T>::~Grid2() {

}

template<class  T>
const Vector2<T>& Grid2<T>::resolution()const {
	return _resolution;
}


template<class  T>
const Vector2<T>& Grid2<T>::origin()const {
	return _origin;
}


template<class  T>
const Vector2<T>& Grid2<T>::gridSpacing()const {
	return _gridSpacing;
}


//注意欣赏这里精妙的匿名函数！
template<class  T>
Grid2<T>::DataPositionFunc Grid2<T>::cellCenterPosition() const {
	return [h, o](size_t i, size_t j) {
		return o + h * Vector2<T>(i + 0.5, j + 0.5);
	};
}

//template<class  T>
//template<typename Callback>
//void Grid2<T>::forEachCellIndex(Callback& func) const {
//	for(size_t j = 0;j<_res)
//}

#endif