#include <iostream>
#include <fstream>
#include <cmath>
#include <string>


#include "../titmouse2d/src/Geometry/Box2.h"
#include "../titmouse2d/src/OtherMethod/RigidBody/RigidBodySolver2.h"
#include "../titmouse2d/src/Matrix2x2.hpp"

#include <GL/glut.h>
#include <cmath>
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


double dt = 0.01;
auto box1 = std::make_shared<Box2>(Vector2D(0.8, 0.8), Vector2D(1.2, 1.2));
auto rigidSolver = std::make_shared<RigidBodySolver2>(box1);

static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	auto& rigidObj = rigidSolver->rigidBodyData->rigidBodyList;
	auto rigidPos = rigidSolver->rigidBodyData->position;

	drawPoint(rigidPos.x, rigidPos.y);

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


	Matrix2x2<int> a(1, 3, 2, 5);
	Matrix2x2<int> b(0, 1, 6, 4);

	auto c = a * b;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			std::cout << c(i, j) << std::endl;
		}
	}

	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();



	return 0;
}


















