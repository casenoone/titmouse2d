#include <iostream>
using namespace std;

#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "../titmouse2d/src/Geometry/Box2.h"
#include "../titmouse2d/src/Geometry/Plane2.h"
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
	glPointSize(3.05f);//缺省是1
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

ParticleSystemSolver2 solver;
double dt = 0.0001;
Collider2 collider;

vector<ExplicitSurface2Ptr> surfaceSet;



static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	auto num = solver._particleSystemData->numberOfParticles();
	auto& pos = solver._particleSystemData->positions();
	for (int i = 0; i < num; ++i) {
		drawPoint(pos[i].x, pos[i].y);
	}

	solver.onAdvanceTimeStep(dt);
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
	glutCreateWindow("titmouse");

	glClearColor(0.0, 0.0, 0.0, 1);
	glShadeModel(GL_FLAT);


	int numberOfParticles = 4;
	int resolutionX = 50;
	int resolutionY = 50;
	vector <Vector2D> temp1;
	for (int i = 0; i < numberOfParticles; ++i) {
		auto x = rand() / double(RAND_MAX) + 1.0;
		auto y = rand() / double(RAND_MAX) + 1.0;
		Vector2D temp(x, y);
		temp1.push_back(temp);
	}

	Array<Vector2D> pos(temp1);



	Box2Ptr box1 = make_shared<Box2>(Vector2D(0, 0), Vector2D(1.99, 1.99), true);
	Box2Ptr box2 = make_shared<Box2>(Vector2D(1, 0), Vector2D(1.3, 0.8), false);
	Plane2Ptr plane1 = make_shared<Plane2>(Vector2D(0.5, 0), Vector2D(1, 0.8), false);

	surfaceSet.push_back(box1);
	surfaceSet.push_back(box2);
	surfaceSet.push_back(plane1);


	collider.push(box1);
	collider.push(box2);
	collider.push(plane1);

	solver.setCollider(collider);
	solver.setData(numberOfParticles, pos, resolutionX, resolutionY);






	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗口显示时

	glutMainLoop();



	return 0;
}


















