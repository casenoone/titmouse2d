#include <iostream>
using namespace std;

#include <fstream>
#include <cmath>
#include <string>
#include <vector>

#include "../titmouse2d/src/Geometry/Sphere2.h"
#include "../titmouse2d/src/Lagrangian/VortexParticleSystemSolver2.h"

#include <GL/glut.h>

#include <windows.h>


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
	glPointSize(2.05f);//缺省是1
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




Vector2<int> resolution(17, 17);
Vector2<double> origin(0.0, 0.0);

Vector2D center1(0.6, 1.0);
double r1 = 0.2;

auto sphere1 = make_shared<Sphere2>(center1, r1, resolution);

auto explicitSphere1 = sphere1->transformToExplicitSurface();

auto vpSolver = make_shared<VortexParticleSystemSolver2>();


double dt = 0.02;

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	vpSolver->onAdvanceTimeStep(dt);

	int numberOfParticles = vpSolver->vortexParticleData()->numberOfParticles();

	for (int i = 0; i < numberOfParticles; ++i) {

		auto pos = vpSolver->vortexParticleData()->positions();
		drawPoint(pos[i].x, pos[i].y);
	}



	for (auto i = explicitSphere1->_data.begin(); i != explicitSphere1->_data.end(); ++i) {
		auto start = i->start;
		auto end = i->end;
		drawLine(start.x, start.y, end.x, end.y);

		////可视化法线
		//auto midPoint = 0.5 * (start + end);
		//auto normalEnd = midPoint + 0.2 * i->normal;
		//drawLine(midPoint.x, midPoint.y, normalEnd.x, normalEnd.y);
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

	vpSolver->emitParticlesFromPanel();

	UINT timerId = 1;
	MSG msg;
	SetTimer(NULL, timerId, 3000, TimerProc);

	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	glutMainLoop();




	return 0;
}


void CALLBACK TimerProc(HWND hwnd, UINT Msg, UINT idEvent, DWORD dwTime)
{
	vpSolver->emitParticlesFromPanel();
	cout << "当前系统粒子数：" << vpSolver->vortexParticleData()->numberOfParticles() << endl;
}