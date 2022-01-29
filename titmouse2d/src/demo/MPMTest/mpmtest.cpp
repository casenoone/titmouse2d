#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;


#include "../../Lagrangian/ParticleSystemSolver2.h"
#include "../../Geometry/Box2.h"
#include "../../Geometry/Plane2.h"

#include "../../Hybrid/ApicSolver2.h"

#include "../../Matrix2x2.hpp"
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


void drawColliders(const vector<ExplicitSurface2Ptr>& surfaceSet) {
	for (auto i = surfaceSet.begin(); i != surfaceSet.end(); ++i) {
		auto surface = (*i)->_data;
		for (auto j = surface.begin(); j != surface.end(); ++j) {
			auto start = j->start;
			auto end = j->end;

			drawLine(start.x, start.y, end.x, end.y);
		}
	}
}


auto apicSolver = ApicSolver2::builder()
.withOrigin(Vector2<double>(0.0, 0.0))
.withResolution(Vector2<size_t>(20, 20))
.makeShared();


double dt = 0.02;
Collider2 collider;

vector<ExplicitSurface2Ptr> surfaceSet;



static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);



	auto num = apicSolver->particleSystemData()->numberOfParticles();
	auto& pos = apicSolver->particleSystemData()->positions();
	for (size_t i = 0; i < num; ++i) {
		drawPoint(pos[i].x, pos[i].y);
	}



	apicSolver->onAdvanceTimeStep(dt);
	drawColliders(surfaceSet);
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


	int numberOfParticles = 400;
	int resolutionX = 10;
	int resolutionY = 10;
	vector <Vector2<double>> temp1;
	for (int i = 0; i < numberOfParticles; ++i) {
		auto x = rand() / double(RAND_MAX) + 0.3;
		auto y = rand() / double(RAND_MAX) + 0.95;
		Vector2<double> temp(x, y);
		temp1.push_back(temp);
	}

	ArrayPtr<Vector2<double>> pos(temp1);


	Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(2.0, 2.0), true);
	Box2Ptr box2 = make_shared<Box2>(Vector2<double>(0.1, 0.1), Vector2<double>(1.9, 1.9), true);
	Box2Ptr box3 = make_shared<Box2>(Vector2<double>(0.6, 0.6), Vector2<double>(1.0, 0.7), false);
	Plane2Ptr plane1 = make_shared<Plane2>(Vector2<double>(0.7, 0.8), Vector2<double>(1.0, 0.8), false);

	surfaceSet.push_back(box1);
	//surfaceSet.push_back(box2);
	//surfaceSet.push_back(box3);
	//surfaceSet.push_back(plane1);


	collider.push(box1);
	//collider.push(box2);
	//collider.push(box3);
	//collider.push(plane1);

	apicSolver->setCollider(collider);
	apicSolver->setData(numberOfParticles, pos, resolutionX, resolutionY);


	Matrix2x2<double> mat(1.0, 2.0, 3.0, 4.0);
	Vector2<double> vec(5.0, 6.0);
	auto vecNew = mat * vec;
	cout << vecNew.x << " " << vecNew.y << endl;
	double num1 = 2;



	//
	//
	//	//这里是写入文件
	////记得重新算的时候要删掉 原来的文件夹
	//	int frame = 1000;
	//	auto num = apicSolver->particleSystemData()->numberOfParticles();
	//	auto position = apicSolver->particleSystemData()->positions();
	//
	//
	//	int interval = 1;
	//
	//	string outfilename = "1";
	//
	//	system("mkdir ApicData1");
	//
	//	for (int i = 0; i < frame; i += 1) {
	//
	//		ofstream out("../titmouse2d/ApicData1/" + outfilename + ".txt", ios::app);
	//
	//		for (int n = 0; n < num; ++n) {
	//			auto x = position[n].x;
	//			auto y = position[n].y;
	//			out << x << "," << y << endl;
	//		}
	//		apicSolver->onAdvanceTimeStep(dt);
	//		auto temp1 = std::atoi(outfilename.c_str());
	//		temp1++;
	//		outfilename = std::to_string(temp1);
	//
	//	}
	//
















	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();



	return 0;
}


















