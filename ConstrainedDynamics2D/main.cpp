#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <GL/glut.h>
#include <vector>
#include "ConstrainedSolver2.h"
#include "../titmouse2d/src/random.h"
#include "../titmouse2d/src/Eulerian/CellCenteredScalarGrid2.h"
#include "../titmouse2d/src/Geometry/Heart2.h"

const float SCREEN_SIZE = 400;
const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;
static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //��ESC
	case 'q': //��q�����򶼻��˳�
		exit(0);
		break;
	}

	glutPostRedisplay();  //�����ػ�ص�
}

void drawPoint(double x, double y)
{
	//�ں󻺴����ͼ�Σ���һ����
	glPointSize(5.0f);//ȱʡ��1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glColor3f(1.0, 0.0, 0.0);

	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
	glEnd();
}

void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(1);//�����߶ο��
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glColor3f(1, 128.0 / 255, 51.0 / 255);

	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //�������귶Χ
	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
	glEnd();
	glFlush();
}


std::shared_ptr<ConstrainedSolver2> constSolver;
double dt = 0.01;
int n;
std::vector<ExplicitSurface2Ptr> surfaceSet;

static void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	/*************************���³������߼�***************************/
	constSolver->onAdvanceTimeStep(dt);

	/*************************���ϳ������߼�***************************/
	//std::cout << 888 << std::endl;
	//�����ʵ�
	for (int i = 0; i < n; ++i) {
		auto& pos = constSolver->massSpringData->positions;
		drawPoint(pos[i].x, pos[i].y);
	}

	//����Լ��
	auto& edge = constSolver->massSpringData->edges;
	auto edgeNum = edge.dataSize();
	for (int i = 0; i < edgeNum; ++i) {
		auto& pos = constSolver->massSpringData->positions;
		auto pi = edge[i].i;
		auto pj = edge[i].j;
		drawLine(pos[pi].x, pos[pi].y, pos[pj].x, pos[pj].y);
	}

	glutSwapBuffers();

}

static void idle(void)
{

	glutPostRedisplay();

}

static void resize(int width, int height)
{
	const float ar = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ratio = 0.1;
	glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //����ʹ������ͶӰ
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glShadeModel(GL_FLAT);


	/*************************���³������߼�***************************/
	n = 0;
	std::vector<Vector2D> pos(n);

	/*for (int i = 0; i < n; ++i) {
		pos[i].x = random_double(0.1, 0.7);
		pos[i].y = random_double(0.1, 0.7);
	}*/

	/*auto grid = CellCenteredScalarGrid2::builder()
		.withOrigin(0, 0)
		.withResolution(30, 30)
		.makeShared();


	Vector2D tempC(1, 1.8);
	for (int i = 0; i < grid->resolution().x; ++i) {
		for (int j = 0; j < grid->resolution().y; ++j) {
			auto c_pos = (grid->dataPosition())(i, j);
			if (c_pos.dis(tempC) < 0.2) {

				pos.push_back(c_pos);
				n++;
			}
		}
	}*/

	Heart2 heart(Vector2D(1, 1), 0.1, Vector2I(30, 30), Vector2D::zero(), 0);
	auto& heart_sdf = heart.sdf();
	auto heart_res = heart_sdf->resolution();
	for (int i = 0; i < heart_res.x; ++i) {
		for (int j = 0; j < heart_res.y; ++j) {
			if (heart_sdf->lookAt(i, j) < 0) {
				auto cell_center = heart_sdf->cellCenterPosition()(i, j);
				pos.push_back(cell_center);
				n++;
			}
		}
	}

	constSolver = std::make_shared<ConstrainedSolver2>(pos);
	//constSolver->massSpringData->restLen = grid->gridSpacing().x;
	//std::cout << "�ʵ���:" << grid->gridSpacing().x << std::endl;
	/*************************���ϳ������߼�***************************/

	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�����ʾʱ

	glutMainLoop();

	//������д���ļ�
//�ǵ��������ʱ��Ҫɾ�� ԭ�����ļ���
	int frame = 10000;
	auto num = constSolver->massSpringData->numberOfPoint;
	auto position = constSolver->massSpringData->positions;


	int interval = 1;

	std::string outfilename = "1";

	//system("mkdir FlipData3");

	for (int i = 0; i < frame; i += 1) {

		std::ofstream out("E:\\zhangjian\\solve_data\\test520_1\\" + outfilename + ".txt", std::ios::app);

		for (int n = 0; n < num; ++n) {
			auto x = position[n].x;
			auto y = position[n].y;
			out << x << "," << y << std::endl;
		}
		constSolver->onAdvanceTimeStep(dt);
		auto temp1 = std::atoi(outfilename.c_str());
		temp1++;
		outfilename = std::to_string(temp1);
		std::cout << "��ǰ���㵽��" << temp1 << "֡" << std::endl;
	}


	return 0;
}
