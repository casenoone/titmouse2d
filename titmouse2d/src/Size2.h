#ifndef SIZE2_H
#define SIZE2_H


//error LNK2001: unresolved external symbol "public: int __thiscall A::max(void)" (?max@A@@QAEHXZ)main.obj
//�Ҳ��������Ķ��壬�����������������ڳ����ж��岻ֹһ�Σ�ֻҪ inline �����Ķ�����ĳ��Դ�ļ���ֻ����һ�Σ�����������Դ�ļ��У��䶨���������ȫ��ͬ�ľͿ��ԡ�

//����ע����������������
//������������������ʵ�ַ�һ��



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
