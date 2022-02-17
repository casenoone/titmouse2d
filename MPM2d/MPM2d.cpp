#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;


#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "../titmouse2d/src/Geometry/Box2.h"
#include "../titmouse2d/src/Geometry/Plane2.h"

#include "../titmouse2d/src/OtherMethod/MPM/MPMSolver2.h"
#include "../titmouse2d/src/Matrix2x2.hpp"
#include <GL/glut.h>
#include <cmath>

#include "../titmouse2d/src/random.h"

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
	glPointSize(3.0f);//缺省是1
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



Vector2<size_t> mpm_res(30, 30);
Vector2D mpm_gs(2.0 / mpm_res.x, 2.0 / mpm_res.x);
Vector2D mpm_origin(0.0, 0.0);

auto mpmSolver = make_shared<MPMSolver2>(mpm_res, mpm_gs, mpm_origin);

double dt = 2e-4;


static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


	auto num = mpmSolver->mpmData()->numberOfParticles();
	auto& pos = mpmSolver->mpmData()->positions();
	for (size_t i = 0; i < num; ++i) {
		drawPoint(pos[i].x, pos[i].y);
	}

	mpmSolver->onAdvanceTimeStep(dt);
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


	int numberOfParticles = 1000;
	int resolutionX = 10;
	int resolutionY = 10;
	vector <Vector2<double>> temp1;
	for (int i = 0; i < numberOfParticles; ++i) {
		auto x = random_double(0.7, 1.3);
		auto y = random_double(0.1, 1.9);
		Vector2<double> temp(x, y);
		temp1.push_back(temp);
	}

	ArrayPtr<Vector2<double>> pos(temp1);


	Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(2.0, 2.0), true);
	Box2Ptr box2 = make_shared<Box2>(Vector2<double>(0.1, 0.1), Vector2<double>(1.9, 1.9), true);
	Box2Ptr box3 = make_shared<Box2>(Vector2<double>(0.6, 0.6), Vector2<double>(1.0, 0.7), false);
	Plane2Ptr plane1 = make_shared<Plane2>(Vector2<double>(0.7, 0.8), Vector2<double>(1.0, 0.8), false);



	mpmSolver->setData(numberOfParticles, pos, resolutionX, resolutionY);

	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();



	return 0;
}


















