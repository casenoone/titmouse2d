#ifndef SIZE2_H
#define SIZE2_H


//error LNK2001: unresolved external symbol "public: int __thiscall A::max(void)" (?max@A@@QAEHXZ)main.obj
//找不到函数的定义，所以内联函数可以在程序中定义不止一次，只要 inline 函数的定义在某个源文件中只出现一次，而且在所有源文件中，其定义必须是完全相同的就可以。

//这里注意对内联函数的理解
//把内联函数的声明与实现放一起



class Size2 {
public:
	Size2();
	~Size2();

	Size2(int _x, int _y);

	bool operator==(const Size2 r) const;

public:
	int x;
	int y;
};

inline bool Size2::operator==(const Size2 r) const {
	if (this->x == r.x && this->y == r.y) {
		return true;
	}
	return false;
}

#endif
