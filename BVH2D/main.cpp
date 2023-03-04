#include <fstream>

#include <iostream>
#include "Vector2.hpp"
#include "Sphere2.h"
#include "BVH2.h"
#include "random.h"
#include "IntersectTestSystem.h"

using namespace std;

#include <GL/glut.h>
#include <cmath>

const float SCREEN_SIZE = 400;
const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;
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
	glPointSize(2.5f);//缺省是1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
	glEnd();
}

void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(1);//设置线段宽度
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //定点坐标范围
	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
	glEnd();
	glFlush();
}



void drawCircle(const Vector2D& center, double r, int res) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * 3.1415926 / res * i)) + center.x;
		auto y = (r * sin(2 * 3.1415926 / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(1, 128.0 / 255, 51.0 / 255);
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * 3.1415926 / res * i)) + center.x;
		auto y = (r * sin(2 * 3.1415926 / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(0, 0, 0);
		glVertex2f(x, y);
	}
	glEnd();
}


void drawCircles(vector<Geometry2Ptr>& sphereSet) {
	for (auto& i : sphereSet) {
		auto new_p = std::dynamic_pointer_cast<Sphere2>(i);
		drawCircle(new_p->center, new_p->r, 20);
	}
}

void drawRay(const Ray2& ray) {
	drawCircle(ray.origin, 0.1, 20);
	auto end = ray.origin + 10.0 * ray.direction;
	drawLine(ray.origin.x, ray.origin.y, end.x, end.y);
}

int n = 100;
std::vector<Geometry2Ptr> sphereSet(n);
Ray2 ray(Vector2D(-1, 2), Vector2D(1, -1));
static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	drawCircles(sphereSet);
	drawRay(ray);
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
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glShadeModel(GL_FLAT);




	for (int i = 0; i < n; ++i) {
		//auto x = rand() / double(RAND_MAX);
		//auto y = rand() / double(RAND_MAX);

		auto x = random_double(0, 2);
		auto y = random_double(0, 2);

		sphereSet[i] = std::make_shared<Sphere2>(Vector2D(x, y), 0.04);
	}


	//将数据写入excel表中，通过对照球体编号来验证BVH是否写对


	ofstream outfile("bvh.csv", ios::trunc);

	IntersectTestSystem sys(sphereSet);
	auto list = sys.findHitObj(ray);
	std::cout << list.size() << endl;
	for (auto i : list) {
		std::cout << "BVH相交序号：" << i << std::endl;
		outfile << i << endl;
	}



	ofstream outfile1("violent.csv", ios::trunc);

	auto list1 = sys.findHitObj_Violent(ray);
	std::cout << list1.size() << endl;
	for (auto i : list1) {
		std::cout << "暴力方法相交序号：" << i << std::endl;
		outfile1 << i << endl;
	}





	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();




	return 0;
}


















