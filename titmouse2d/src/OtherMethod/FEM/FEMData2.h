#include "../../mesh/Mesh2.h"
#include "../../Matrix2x2.hpp"

class FEMData2 {
public:
	FEMData2(std::initializer_list<Vector2D> vertexs, std::initializer_list<int> indexs) {
		mesh = std::make_shared<Mesh2>(vertexs, indexs);
		forces.resize(vertexs.size());
		velocities.resize(forces.size());
		originEdgeMatrix.resize(indexs.size() / 3);
		deformGradMatrix.resize(indexs.size() / 3);
		strainMatrix.resize(indexs.size() / 3);
	}

public:
	Mesh2Ptr mesh;

	std::vector<Vector2D> forces;
	std::vector<Vector2D> velocities;

	//ԭʼ�ı���������
	std::vector<Matrix2x2<double>> originEdgeMatrix;

	//�α��ݶȾ���
	std::vector<Matrix2x2<double>> deformGradMatrix;

	//S���󣬲�֪���������ɶ
	std::vector<Matrix2x2<double>> strainMatrix;

	//�����ܶȺ����Ĳ���
	double mu = 0.3;
	double lambda = 8000;
};

typedef std::shared_ptr<FEMData2> FEMData2Ptr;
