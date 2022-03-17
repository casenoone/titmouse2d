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
	glPointSize(2.05f);//ȱʡ��1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glVertex3f((x - 1) * 10, (y - 1) * 10, 0);
	glEnd();
}





void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(1);//�����߶ο��
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //�������귶Χ
	glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
	glEnd();
	glFlush();
}




Vector2I resolution(25, 25);
Vector2D origin(0.0, 0.0);

Vector2D center1(0.6, 1.0);
double r1 = 0.2;




auto sphere1 = make_shared<Sphere2>(center1, r1, resolution);

auto explicitSphere1 = sphere1->transformToExplicitSurface();

auto vpSolver = make_shared<FoamVortexSolver>();

auto sphereBox = sphere1->boundingBox();

double movingCoffe = 0.1;

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

		////���ӻ�����
		//auto midPoint = 0.5 * (start + end);
		auto midPoint = explicitSphere1->midPoint(m++);
		auto normalEnd = midPoint + 0.2 * i->normal;
		//drawLine(midPoint.x, midPoint.y, normalEnd.x, normalEnd.y);
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
	glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //����ʹ������ͶӰ
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



	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�zz����ʾʱ

	glutMainLoop();




	return 0;
}


void CALLBACK TimerProc(HWND hwnd, UINT Msg, UINT idEvent, DWORD dwTime)
{
	auto num = vpSolver->foamVortexData()->numberOfParticles();
	if (num < 1) {
		vpSolver->emitParticlesFromPanel();
		cout << "��ǰϵͳ��������" << num << endl;
	}
}