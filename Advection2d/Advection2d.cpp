#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

#include "../titmouse2d/src/ConstVar.h"

#include <GL/glut.h>
#include <cmath>

#include "../titmouse2d/src/Color3.hpp"
#include "../titmouse2d/src/Vector2.hpp"
#include "../titmouse2d/src/Eulerian/CellCenteredVectorGrid2.hpp"
#include "../titmouse2d/src/Eulerian/FaceCenteredGrid2.h"
#include "../titmouse2d/src/Eulerian/AdvectionSolver2.h"

//advection测试
//测试场景：

//一个旋转的圆形，其中一半是黑色，一半是白色
//方法：线性后向追踪、中点法
//速度场的存储：使用MAC网格
//颜色场存于中心网格



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

void drawPoint(Vector2D pos, float size, Color3<float> color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r / 255.0, color.g / 255.0, color.b / 255.0);
	glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
	glEnd();
}

void drawPoint(Vector2D pos, float size, Color3<double> color)
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


Vector2I resolution(40, 40);
Vector2D origin(0.0, 0.0);
Color3<double> red(255, 0.0, 0.0);

CellCenteredVectorGrid2Ptr<Color3<double>> advectedData;


Vector2D center1(1.0, 1.0);
double r1 = 0.5;
double r1_sqr = r1 * r1;

Color3<double> blacks(0, 0, 0);
Color3<double> whites(255, 255, 255);


auto velocity = std::make_shared<FaceCenteredGrid2>(resolution, origin, Vector2D(0, 0));

auto advectionSolver = std::make_shared<AdvectionSolver2>();

static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


	for (int i = 0; i < advectedData->resolution().x; ++i) {
		for (int j = 0; j < advectedData->resolution().y; ++j) {
			auto dataFunc = advectedData->dataPosition();
			auto pos = dataFunc(i, j);
			auto color = (*advectedData)(i, j);
			drawPoint(pos, 3.5f, color);
		}
	}

	advectionSolver->solve(velocity, advectedData, 0.1);

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





	advectedData = std::make_shared<CellCenteredVectorGrid2<Color3<double>>>(resolution, origin, red);

	for (int i = 0; i < advectedData->resolution().x; ++i) {
		for (int j = 0; j < advectedData->resolution().y; ++j) {
			auto posFunc = advectedData->dataPosition();
			auto current_pos = posFunc(i, j);
			if (current_pos.disSquare(center1) <= r1_sqr) {
				if (current_pos.x - center1.x <= 0.0) (*advectedData)(i, j) = blacks;
				else (*advectedData)(i, j) = whites;

			}
		}
	}



	//设置一个旋转的速度场
	auto velSizeU = velocity->uSize();

	for (int i = 0; i < velSizeU.x; ++i) {
		for (int j = 0; j < velSizeU.y; ++j) {
			auto posFunc = velocity->uPosition();
			auto pos = posFunc(i, j);
			auto tempVec = (pos - center1);
			velocity->u(i, j) = tempVec.x * cos(kPiD / 2) - tempVec.y * sin(kPiD / 2);
		}
	}

	auto velSizeV = velocity->vSize();

	for (int i = 0; i < velSizeV.x; ++i) {
		for (int j = 0; j < velSizeV.y; ++j) {
			auto posFunc = velocity->vPosition();
			auto pos = posFunc(i, j);
			auto tempVec = (pos - center1);
			velocity->v(i, j) = tempVec.x * sin(kPiD / 2) + tempVec.y * cos(kPiD / 2);
		}
	}



	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	glutMainLoop();



	return 0;
}


















