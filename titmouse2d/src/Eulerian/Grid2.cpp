
#include "Grid2.h"

Grid2::Grid2() {

}

Grid2::~Grid2() {

}


const Vector2I& Grid2::resolution()const {
	return _resolution;
}


const Vector2D& Grid2::origin()const {
	return _origin;
}



const Vector2D& Grid2::gridSpacing()const {
	return _gridSpacing;
}





//注意欣赏这里精妙的匿名函数！

Grid2::DataPositionFunc Grid2::cellCenterPosition() const {
	auto h = _gridSpacing;
	auto o = _origin;
	return [h, o](int i, int j) {
		return o + h * Vector2D(i + 0.5, j + 0.5);
	};
}

//template<typename Callback>
//void Grid2<T>::forEachCellIndex(Callback& func) const {
//	for(int j = 0;j<_res)
//}
