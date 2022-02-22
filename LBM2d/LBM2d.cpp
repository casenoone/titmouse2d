#include <iostream>
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


#include "../titmouse2d/src/OtherMethod/LBM/LBMSolver2.h"

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

void drawPoint(Vector2<double> pos, float size, Color3<float> color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor4f(color.r, color.g, color.b, 0);
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

const double minVel = 0;
const double maxVel = 0.1;


Vector2<int> resolution(40, 40);
auto LBMSolver = make_shared<LBMSolver2>(resolution);
auto spacing = 2.0 / resolution.x;
auto halfSpacing = 0.5 * spacing;

//障碍物
vector<vector<int>> temp1;

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	LBMSolver->onAdvancedTimeStep();

	auto rho = LBMSolver->getRho();

	float pointSize = 4.5f;
	//可视化部分
	for (int i = 0; i < resolution.x; ++i) {
		for (int j = 0; j < resolution.y; ++j) {
			auto currentX = Vector2<double>(i, j) * spacing +
				Vector2<double>(halfSpacing, halfSpacing);

			auto temp = LBMSolver->getGridState(i, j);
			if (temp != LBM_OBS) {
				auto u_x = LBMSolver->velocityAt(i, j).x;
				auto u_y = LBMSolver->velocityAt(i, j).y;

				if (u_x < 0)u_x *= -1;
				if (u_y < 0)u_y *= -1;

				double k1 = 1;

				int r = (int)((u_x - minVel) / (maxVel - minVel) * 255.0) * k1;
				if (r < 0) r = 0;
				if (r > 255) r = 255;
				int g = (int)((u_y - minVel) / (maxVel - minVel) * 255.0) * k1;
				if (g < 0) g = 0;
				if (g > 255) g = 255;
				int b = 0;

				Color3<double> color(r, g, b);
				drawPoint(currentX, pointSize, color);

			}
		}
	}


	for (int i = 0; i < resolution.x; ++i) {
		for (int j = 0; j < resolution.y; ++j) {
			auto currentX = Vector2<double>(i, j) * spacing +
				Vector2<double>(halfSpacing, halfSpacing);
			auto temp = LBMSolver->getGridState(i, j);
			if (temp == LBM_VELOCITY)
				drawPoint(currentX, pointSize, Color3<float>(1, 0.4, 0));

			if (temp == LBM_OBS)
				drawPoint(currentX, pointSize, Color3<float>(0.5, 0.7, 0));

		}
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	//glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	float color1 = 0;
	glClearColor(color1, color1, color1, 1);
	glShadeModel(GL_FLAT);


	//设置一个圆形的障碍物

	double r1 = 0.2;
	Vector2<double> center1(0.4, 1.0);

	auto res = resolution;

	temp1.resize(res.x);

	for (int i = 0; i < res.x; ++i) {
		temp1[i].resize(res.y);
		for (int j = 0; j < res.y; ++j) {
			auto currentX = Vector2<double>(i, j) * spacing +
				Vector2<double>(halfSpacing, halfSpacing);

			//cout << currentX.x << "," << currentX.y << "," << (currentX - center1).getLength() << endl;
			if ((currentX - center1).getLength() < r1) {
				temp1[i][j] = LBM_OBS;
			}
			else {
				temp1[i][j] = LBM_FLUID;
			}

		}
	}

	Array2<int> lbm_collider(temp1);

	//LBMSolver->setCollider(lbm_collider);




	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	glutMainLoop();








	return 0;
}

