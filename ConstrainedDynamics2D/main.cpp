#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <GL/glut.h>
#include <vector>
#include "ConstrainedSolver2.h"
#include "../titmouse2d/src/random.h"
#include "../titmouse2d/src/Eulerian/CellCenteredScalarGrid2.h"

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


std::shared_ptr<ConstrainedSolver2> constSolver;
double dt = 0.01;
int n;
static void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	/*************************以下程序主逻辑***************************/
	constSolver->onAdvanceTimeStep(dt);

	/*************************以上程序主逻辑***************************/
	//std::cout << 888 << std::endl;
	//绘制质点
	for (int i = 0; i < n; ++i) {
		auto& pos = constSolver->massSpringData->positions;
		drawPoint(pos[i].x, pos[i].y);
	}

	//绘制约束
	auto& edge = constSolver->massSpringData->edges;
	auto edgeNum = edge.dataSize();
	for (int i = 0; i < edgeNum; ++i) {
		auto& pos = constSolver->massSpringData->positions;
		auto pi = edge[i].i;
		auto pj = edge[i].j;
		drawLine(pos[pi].x, pos[pi].y, pos[pj].x, pos[pj].y);
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
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glShadeModel(GL_FLAT);


	/*************************以下程序主逻辑***************************/
	n = 0;
	std::vector<Vector2D> pos(n);

	/*for (int i = 0; i < n; ++i) {
		pos[i].x = random_double(0.1, 0.7);
		pos[i].y = random_double(0.1, 0.7);
	}*/

	auto grid = CellCenteredScalarGrid2::builder()
		.withOrigin(0, 0)
		.withResolution(30, 30)
		.makeShared();


	Vector2D tempC(1, 1);
	for (int i = 0; i < grid->resolution().x; ++i) {
		for (int j = 0; j < grid->resolution().y; ++j) {
			auto c_pos = (grid->dataPosition())(i, j);
			if (c_pos.dis(tempC) < 0.17) {

				pos.push_back(c_pos);
				n++;
			}
		}
	}

	constSolver = std::make_shared<ConstrainedSolver2>(pos);
	//constSolver->massSpringData->restLen = grid->gridSpacing().x;
	std::cout << "质点间距:" << grid->gridSpacing().x << std::endl;
	/*************************以上程序主逻辑***************************/

	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();


	return 0;
}


















