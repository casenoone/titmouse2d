﻿#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

#include "../titmouse2d/src/ConstVar.h"
#include "../titmouse2d/src/random.h"
#include <GL/glut.h>
#include <cmath>

#include "../titmouse2d/src/Color3.hpp"
#include "../titmouse2d/src/Vector2.hpp"

#include "../titmouse2d/src/Lagrangian/VortexParticleSystemSolver2.h"

#include "../titmouse2d/src/Geometry/Box2.h"
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

void drawPoint(Vector2<double> pos, float size, Color3<float> color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r / 255.0, color.g / 255.0, color.b / 255.0);
	glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
	glEnd();
}

void drawPoint(Vector2<double> pos, float size, Color3<double> color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(float(color.r) / 255.0, float(color.g) / 255.0, float(color.b) / 255.0);
	glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
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


void drawLine(double x1, double y1, double x2, double y2, Color3<float> color) {

	glLineWidth(1);//设置线段宽度
	glBegin(GL_LINES);
	glColor3f(color.r / 255.0, color.g / 255.0, color.b / 255.0);
	glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //定点坐标范围
	glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
	glEnd();
	glFlush();
}

auto vpSolver = make_shared<VortexParticleSystemSolver2>();

int numberOfparticles = 300;

double dt = 0.02;

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	vpSolver->onAdvanceTimeStep(dt);

	for (int i = 0; i < numberOfparticles; ++i) {

		auto pos = vpSolver->vortexParticleData()->positions();
		drawPoint(pos[i].x, pos[i].y);
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

	int res_x = 30;
	int res_y = res_x;

	vector<Vector2<double>> tempPos;

	for (int i = 0; i < numberOfparticles; ++i) {
		auto x = random_double(0.7, 1.3);
		auto y = random_double(0.1, 0.4);
		Vector2<double> temp(x, y);
		tempPos.push_back(temp);
	}

	Array<Vector2<double>> pos(tempPos);


	vpSolver->setData(numberOfparticles, pos, res_x, res_y);

	Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(2.0, 2.0), true);
	Collider2 collider;
	collider.push(box1);
	//vpSolver->setCollider(collider);



	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	glutMainLoop();



	return 0;
}