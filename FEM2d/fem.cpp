#include <iostream>
#include <fstream>
#include <cmath>
#include <string>


#include "../titmouse2d/src/OtherMethod/FEM/FEMSolver2.h"
#include "../titmouse2d/src/mesh/Mesh2.h"

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


double dt = 0.004;

//std::initializer_list<Vector2D> pos = { Vector2D(1,1),Vector2D(1,0.8),Vector2D(1.2,0.8),Vector2D(1.2,1) };
//std::initializer_list<int> index = { 0,1,2,0,2,3 };
std::initializer_list<Vector2D> pos = { Vector2D(1,1),Vector2D(1,0.8),Vector2D(1.2,0.8) };
std::initializer_list<int> index = { 0,1,2 };
//std::initializer_list<Vector2D> pos = { Vector2D(1,1),Vector2D(1.2,1),Vector2D(1.2,0.8) };
//std::initializer_list<int> index = { 0,1,2 };
//std::initializer_list<Vector2D> pos = { Vector2D(0.8,0.8),Vector2D(1,1),Vector2D(1.2,0.8) };
//std::initializer_list<int> index = { 0,1,2 };

FEMSolver2 fem(pos, index);
auto& data = fem.femData;
auto& mesh = data->mesh;
auto& vertex = mesh->vertexList;
static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
	fem.onAdvanceTimeStep(dt);

	for (int i = 0; i < mesh->size(); i++) {
		auto i0 = mesh->triList[i].index[0];
		auto i1 = mesh->triList[i].index[1];
		auto i2 = mesh->triList[i].index[2];

		drawLine(vertex[i0].x, vertex[i0].y, vertex[i1].x, vertex[i1].y);
		drawLine(vertex[i0].x, vertex[i0].y, vertex[i2].x, vertex[i2].y);
		drawLine(vertex[i2].x, vertex[i2].y, vertex[i1].x, vertex[i1].y);
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







	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();



	return 0;
}


















