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


double dt = 0.0010;

//std::initializer_list<Vector2D> pos = { Vector2D(1,1),Vector2D(1,0.9),Vector2D(1.1,0.9),Vector2D(1.1,1) };
//std::initializer_list<int> index = { 0,1,2,0,2,3 };
//std::initializer_list<Vector2D> pos = { Vector2D(1,0.5),Vector2D(1,0.40),Vector2D(1.1,0.40) };
//std::initializer_list<int> index = { 0,1,2 };
//std::initializer_list<Vector2D> pos = { Vector2D(1,0.25),Vector2D(1.1,0.25),Vector2D(1.0,0.15) };
//std::initializer_list<int> index = { 0,1,2 };
//std::initializer_list<Vector2D> pos = { Vector2D(0.98,0.4),Vector2D(1,0.5),Vector2D(1.02,0.4) };
//std::initializer_list<int> index = { 0,1,2 };

//1.05
double all_h = 0.5;
double w1 = 0.5;
double w2 = 1.0;
double w3 = 1.5;
int xa = 36;
std::initializer_list<Vector2D> pos = {
Vector2D(0.3,0.0 + all_h),
Vector2D(0.3,0.05 + all_h),
Vector2D(0.05,0.0 + all_h),
Vector2D(0.05,0.05 + all_h),
Vector2D(0,0.05 + all_h),
Vector2D(0,0 + all_h),
Vector2D(0.0,0.3 + all_h),
Vector2D(0.0,0.35 + all_h),
Vector2D(0.05,0.35 + all_h),
Vector2D(0.05,0.3 + all_h),
Vector2D(0.25,0.3 + all_h),
Vector2D(0.25,0.35 + all_h),
Vector2D(0.3,0.35 + all_h),
Vector2D(0.3,0.3 + all_h),
Vector2D(0.3,0.6 + all_h),
Vector2D(0.25,0.6 + all_h),
Vector2D(0.25,0.65 + all_h),
Vector2D(0.3,0.65 + all_h),
Vector2D(0.0,0.65 + all_h),
Vector2D(0.0,0.6 + all_h),


Vector2D(0 + w1,0.65 + all_h),
Vector2D(0.05 + w1,0.65 + all_h),
Vector2D(0 + w1,0.6 + all_h),
Vector2D(0.05 + w1,0.6 + all_h),
Vector2D(0.25 + w1,0.65 + all_h),
Vector2D(0.3 + w1,0.65 + all_h),
Vector2D(0.25 + w1,0.6 + all_h),
Vector2D(0.3 + w1,0.6 + all_h),
Vector2D(0.0 + w1,0.05 + all_h),
Vector2D(0 + w1,0 + all_h),
Vector2D(0.05 + w1,0.05 + all_h),
Vector2D(0.05 + w1,0 + all_h),
Vector2D(0.25 + w1,0.05 + all_h),
Vector2D(0.3 + w1,0.05 + all_h),
Vector2D(0.25 + w1,0 + all_h),
Vector2D(0.3 + w1,0 + all_h),

Vector2D(0.3 + w2,0.0 + all_h),
Vector2D(0.3 + w2,0.05 + all_h),
Vector2D(0.05 + w2,0.0 + all_h),
Vector2D(0.05 + w2,0.05 + all_h),
Vector2D(0 + w2,0.05 + all_h),
Vector2D(0 + w2,0 + all_h),
Vector2D(0.0 + w2,0.3 + all_h),
Vector2D(0.0 + w2,0.35 + all_h),
Vector2D(0.05 + w2,0.35 + all_h),
Vector2D(0.05 + w2,0.3 + all_h),
Vector2D(0.25 + w2,0.3 + all_h),
Vector2D(0.25 + w2,0.35 + all_h),
Vector2D(0.3 + w2,0.35 + all_h),
Vector2D(0.3 + w2,0.3 + all_h),
Vector2D(0.3 + w2,0.6 + all_h),
Vector2D(0.25 + w2,0.6 + all_h),
Vector2D(0.25 + w2,0.65 + all_h),
Vector2D(0.3 + w2,0.65 + all_h),
Vector2D(0.0 + w2,0.65 + all_h),
Vector2D(0.0 + w2,0.6 + all_h),


Vector2D(0 + w3,0.65 + all_h),
Vector2D(0.25 + w3,0.65 + all_h),
Vector2D(0.3 + w3,0.65 + all_h),
Vector2D(0 + w3,0.6 + all_h),
Vector2D(0.25 + w3,0.6 + all_h),
Vector2D(0.3 + w3,0.6 + all_h),
Vector2D(0 + w3,0.35 + all_h),
Vector2D(0.25 + w3,0.35 + all_h),
Vector2D(0.3 + w3,0.35 + all_h),
Vector2D(0 + w3,0.3 + all_h),
Vector2D(0.25 + w3,0.3 + all_h),
Vector2D(0.3 + w3,0.3 + all_h),
Vector2D(0 + w3,0.05 + all_h),
Vector2D(0.25 + w3,0.05 + all_h),
Vector2D(0.3 + w3,0.05 + all_h),
Vector2D(0 + w3,0 + all_h),
Vector2D(0.25 + w3,0 + all_h),
Vector2D(0.3 + w3,0 + all_h)

};
std::initializer_list<int> index = {
1,0,2,
1,2,3,
2,5,3,
5,4,3,
4,6,9,
4,9,3,
7,6,8,
8,6,9,
8,11,9,
11,9,10,
11,10,12,
12,10,13,
11,15,14,
11,14,12,
16,15,17,
17,14,15,
18,16,19,
18,19,15,




20,21,22,
21,22,23,
21,23,24,
24,23,26,
24,25,26,
25,26,27,
22,23,28,
23,28,30,
26,27,32,
27,32,33,
28,30,29,
29,30,31,
30,32,31,
31,32,34,
32,33,34,
33,34,35,

1 + xa,0 + xa,2 + xa,
1 + xa,2 + xa,3 + xa,
2 + xa,5 + xa,3 + xa,
5 + xa,4 + xa,3 + xa,
4 + xa,6 + xa,9 + xa,
4 + xa,9 + xa,3 + xa,
7 + xa,6 + xa,8 + xa,
8 + xa,6 + xa,9 + xa,
8 + xa,11 + xa,9 + xa,
11 + xa,9 + xa,10 + xa,
11 + xa,10 + xa,12 + xa,
12 + xa,10 + xa,13 + xa,
11 + xa,15 + xa,14 + xa,
11 + xa,14 + xa,12 + xa,
16 + xa,15 + xa,17 + xa,
17 + xa,14 + xa,15 + xa,
18 + xa,16 + xa,19 + xa,
18 + xa,19 + xa,15 + xa,

56,57,59,
59,57,60,
57,60,58,
58,60,61,
60,61,63,
61,63,64,
62,63,65,
63,65,66,
63,64,66,
64,66,67,
66,67,69,
67,69,70,
68,69,71,
69,71,72,
69,70,72,
70,72,73,

1,10,13,
19,7,15,

1 + xa,10 + xa,13 + xa,
19 + xa,7 + xa,15 + xa,

59,60,62,
68,65,66

};

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
		if (i0 == 1 && i1 == 10 & i2 == 13) {}
		else if (i0 == 19 && i1 == 7 & i2 == 15) {}
		else if (i0 == 19 + 36 && i1 == 7 + 36 & i2 == 15 + 36) {}
		else if (i0 == 1 + 36 && i1 == 10 + 36 & i2 == 13 + 36) {}
		else if (i0 == 59 && i1 == 60 & i2 == 62) {}
		else if (i0 == 68 && i1 == 65 & i2 == 66) {}
		else {
			drawLine(vertex[i0].x, vertex[i0].y, vertex[i1].x, vertex[i1].y);
			drawLine(vertex[i0].x, vertex[i0].y, vertex[i2].x, vertex[i2].y);
			drawLine(vertex[i2].x, vertex[i2].y, vertex[i1].x, vertex[i1].y);
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
