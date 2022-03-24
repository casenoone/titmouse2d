#include <iostream>
using namespace std;

#include <fstream>
#include <cmath>
#include <string>
#include <vector>

#include "../titmouse2d/src/Geometry/Sphere2.h"
#include "FoamVortexSolver.h"
#include "../titmouse2d/src/SparseMatrix.hpp"
#include "../titmouse2d/src/Eulerian/MarchingCubes2.h"
#include <GL/glut.h>

#include <windows.h>
#include <Eigen/Dense>

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //按ESC
	case 'q': //按q键程序都会退出
		exit(0);
		break;
	}

	glutPostRedisplay();  //产生重绘回调
}

void drawPoint(double x, double y)
{
	//在后缓存绘制图形，就一个点
	glPointSize(4.05f);//缺省是1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glVertex3f((x - 1) * 10, (y - 1) * 10, 0);
	glEnd();
}





void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(1);//设置线段宽度
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //定点坐标范围
	glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
	glEnd();
	glFlush();
}




Vector2I resolution(12, 12);
Vector2D origin(0.0, 0.0);

Vector2D center1(0.9, 1.1);
double r1 = 0.4;




auto sphere1 = make_shared<Sphere2>(center1, r1, resolution);

auto explicitSphere1 = sphere1->transformToExplicitSurface();

auto vpSolver = make_shared<FoamVortexSolver>();

auto sphereBox = sphere1->boundingBox();

double movingCoffe = 0.3;

Vector2I movingGridRes(20, 20);
BoundingBox2 movingGridDomain(sphereBox.lowerCorner - Vector2D(movingCoffe, movingCoffe),
	sphereBox.upperCorner + Vector2D(movingCoffe, movingCoffe));


double dt = 0.02;






static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	vpSolver->onAdvanceTimeStep(dt);

	int numberOfParticles = vpSolver->foamVortexData()->numberOfParticles();

	for (int i = 0; i < numberOfParticles; ++i) {

		auto pos = vpSolver->foamVortexData()->positions();
		drawPoint(pos[i].x, pos[i].y);
	}

	int m = 0;
	for (auto i = explicitSphere1->_data.begin(); i != explicitSphere1->_data.end(); ++i) {
		auto start = i->start;
		auto end = i->end;
		drawLine(start.x, start.y, end.x, end.y);

		////可视化法线
		auto midPoint = explicitSphere1->midPoint(m++);
		auto normalEnd = start + 0.2 * i->normal;
		drawPoint(start.x, start.y);
		//drawLine(start.x, start.y, normalEnd.x, normalEnd.y);
	}

	/*auto movingSize = vpSolver->foamVortexData()->movingGrid->uSize();
	for (int i = 0; i < movingSize.x; ++i) {
		for (int j = 0; j < movingSize.y; ++j) {
			auto posfunc = vpSolver->foamVortexData()->movingGrid->uPosition();
			drawPoint(posfunc(i, j).x, posfunc(i, j).y);
		}
	}*/

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
	glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //这里使用正视投影
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glShadeModel(GL_FLAT);

	vpSolver->setPanels(explicitSphere1);
	vpSolver->setMovingGrid(movingGridRes, movingGridDomain);
	vpSolver->emitParticlesFromPanel();



	UINT timerId = 1;
	MSG msg;
	SetTimer(NULL, timerId, 1, TimerProc);




	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	glutMainLoop();

	Eigen::MatrixXd A(4, 4);
	Eigen::VectorXd b(4);

	A << 1.14689, 1.06625, 1.18281, 1.13222,
		-0.150321, -0.195824, -0.195824, -0.241327,
		-0.397666, -0.337486, -0.337486, -0.277307,
		-1.62209, -1.55612, -1.67269, -1.63677;

	/*A(0, 0) = 1.14689;
	A(0, 1) = 1.06625;
	A(0, 2) = 1.18281;
	A(0, 3) = 1.3222;
	A(1, 0) = -0.150321;
	A(1, 1) = -0.195824;
	A(1, 2) = -0.195824;
	A(1, 3) = -0.241327;
	A(2, 0) = -0.397666;
	A(2, 1) = -0.337486;
	A(2, 2) = -0.337486;
	A(2, 3) = -0.277307;
	A(3, 0) = -1.62209;
	A(3, 1) = -1.55612;
	A(3, 2) = -1.67269;
	A(3, 3) = -1.63677;*/

	/*b[0] = -0.00332102;
	b[1] = 0;
	b[2] = 0;
	b[3] = 0;*/

	b[0] = -0.00332102;
	b[1] = 0;
	b[2] = 0;
	b[3] = 0;

	Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
	cout << x << endl;

	return 0;
}


void CALLBACK TimerProc(HWND hwnd, UINT Msg, UINT idEvent, DWORD dwTime)
{
	auto num = vpSolver->foamVortexData()->numberOfParticles();
	if (num < 1) {
		vpSolver->emitParticlesFromPanel();
		cout << "当前系统粒子数：" << num << endl;
	}
}



