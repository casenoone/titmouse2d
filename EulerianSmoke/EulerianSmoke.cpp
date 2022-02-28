#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

#include "../titmouse2d/src/Color3.hpp"

#include "../titmouse2d/src/Eulerian/EulerianSmoke/EulerianSmokeSolver2.h"

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


void drawPoint(Vector2D pos, Color3<double> color, float size)
{
	//在后缓存绘制图形，就一个点
	glPointSize(size);//缺省是1
	glBegin(GL_POINTS);
	glColor3f(color.r / 255, color.g / 255, color.b / 255);
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




auto smokeSolver = EulerianSmokeSolver2::builder()
.withOrigin(Vector2D(0.0, 0.0))
.withResolution(Vector2I(20, 20))
.makeShared();

double dt = 0.01;




static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	auto size = smokeSolver->resolution();
	auto rho = smokeSolver->eulerianSmokeData()->densities();
	smokeSolver->onAdvanceTimeStep(dt);

	for (int i = 0; i < size.x; ++i) {
		for (int j = 0; j < size.y; ++j) {
			auto posFunc = rho->cellCenterPosition();
			auto pos = posFunc(i, j);
			auto current_rho = rho->lookAt(i, j);

			Color3<double> color(current_rho, current_rho, current_rho);
			drawPoint(pos, color, 3.5);
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
	glutInitWindowSize(200, 200);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	//glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glClearColor(0.0, 0.0, 0.0, 1);
	glShadeModel(GL_FLAT);






	////这里是写入文件
 //  //记得重新算的时候要删掉 原来的文件夹
	//int frame = 1000;

	//string outfilename = "1";

	//int num = 0;

	//system("mkdir EulerianSmoke1");

	//for (int i = 0; i < frame; i += 1) {

	//	ofstream out("../titmouse2d/EulerianSmoke1/" + outfilename + ".txt", ios::app);

	//	auto size = smokeSolver->resolution();
	//	auto rho = smokeSolver->eulerianSmokeData()->densities();
	//	for (int i = 0; i < size.x; ++i) {
	//		for (int j = 0; j < size.y; ++j) {
	//			auto posFunc = rho->cellCenterPosition();
	//			auto pos = posFunc(i, j);

	//			out << pos.x << "," << pos.y << "," << rho->lookAt(i, j) << endl;
	//		}
	//	}


	//	smokeSolver->onAdvanceTimeStep(0.008);
	//	auto temp1 = std::atoi(outfilename.c_str());
	//	temp1++;
	//	outfilename = std::to_string(temp1);

	//}














	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();



	return 0;
}


















