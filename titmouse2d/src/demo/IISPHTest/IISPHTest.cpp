#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

#include "../../ConstVar.h"
#include "../../random.h"
#include <GL/glut.h>
#include <cmath>

#include "../../Color3.hpp"
#include "../../Vector2.hpp"

#include "../../Geometry/Box2.h"
#include "../../OtherMethod/IISPH/IISPHSolver2.h"

#include "../../random.h"
#include "../../TempMovingCollider2.h"

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

auto iisphSolver = make_shared<IISphSolver2>();
int numberOfParticles = 0;
static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	iisphSolver->onAdvanceTimeStep(0.01);
	//auto movingPos_x = pbfSolver->movingColliderPos;
	for (int i = 0; i < numberOfParticles; ++i) {
		auto pos = iisphSolver->iisphData()->positions();
		drawPoint(pos[i].x, pos[i].y);
		//drawLine(movingPos_x, 0.0, movingPos_x, 2.0);
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



	int res_x = 20;
	int res_y = 20;

	vector<Vector2<double>> temp_pos;

	numberOfParticles = 400;
	for (int i = 0; i < numberOfParticles; ++i) {
		auto x = random_double(0.8, 1.5);
		auto y = random_double(0.2, 1.5);
		Vector2<double> temp(x, y);
		temp_pos.push_back(temp);
	}

	cout << "粒子数目：" << numberOfParticles << endl;
	ArrayPtr<Vector2<double>> pos(temp_pos);

	iisphSolver->setData(numberOfParticles, pos, res_x, res_y);

	Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(2.0, 2.0), true);

	Box2Ptr box2 = make_shared<Box2>(Vector2<double>(0., 0.2), Vector2<double>(1.8, 0.2), false);


	Collider2 collider;

	collider.push(box1);
	//collider.push(box2);
	iisphSolver->setCollider(collider);






	//这里是写入文件
//记得重新算的时候要删掉 原来的文件夹
	int frame = 1000;
	auto num = iisphSolver->iisphData()->numberOfParticles();
	auto position = iisphSolver->iisphData()->positions();


	int interval = 1;

	string outfilename = "1";

	system("mkdir IISPH3");

	for (int i = 0; i < frame; i += 1) {

		ofstream out("../titmouse2d/IISPH3/" + outfilename + ".txt", ios::app);

		for (int n = 0; n < num; ++n) {
			auto x = position[n].x;
			auto y = position[n].y;
			out << x << "," << y << endl;
		}
		iisphSolver->onAdvanceTimeStep(0.01);
		auto temp1 = std::atoi(outfilename.c_str());
		temp1++;
		outfilename = std::to_string(temp1);

		cout << "当前解算到第" << i << "步，还有" << frame - i << "步" << endl;
	}



	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	//glutMainLoop();








	return 0;
}

