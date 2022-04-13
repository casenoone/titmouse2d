#include <iostream>
using namespace std;

#include <cmath>


#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "../Voronoi2D/Voronoi2.h"
#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
#include "../titmouse2d/src/random.h"
#include "../titmouse2d/src/Geometry/Box2.h"
#include "../titmouse2d/src/Collider2.h"

const float SCREEN_SIZE = 600;
const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;



static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //��ESC
	case 'q': //��q�����򶼻��˳�
		exit(0);
		break;

	}


	glutPostRedisplay();  //�����ػ�ص�
}

void drawPoint(double x, double y)
{
	//�ں󻺴����ͼ�Σ���һ����
	glPointSize(4.05f);//ȱʡ��1
	glBegin(GL_POINTS);
	//glColor3f(1, 128.0 / 255, 51.0 / 255);
	glColor3f(1, 1, 1);
	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
	glEnd();
}





void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(2.5);//�����߶ο��
	glBegin(GL_LINES);
	glColor3f(1, 128.0 / 255, 51.0 / 255);

	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //�������귶Χ
	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
	glEnd();
	glFlush();
}



void drawVoronoi(const Voronoi2& voronoi) {
	auto& edges = voronoi._data.edges;
	auto& sites = voronoi._data.sites;

	auto edgeLength = edges.size();
	for (int i = 0; i < edgeLength; ++i) {
		drawLine(edges[i]->start.x, edges[i]->start.y,
			edges[i]->end.x, edges[i]->end.y);

	}

	for (int i = 0; i < sites.dataSize(); ++i) {
		drawPoint(sites.lookAt(i).x, sites.lookAt(i).y);
	}
}

int number = 3;
int w = 200;
int h = 200;
int sweepY = 0;

int n = 100;

ParticleSystemSolver2 particleSolve;
auto& pos = particleSolve._particleSystemData->positions();
static void display(void)
{



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//����MODELVIEW��������Ϊ��λ���ٳ��Ϲ۲���󣬼���z������100���� ��0��0��0��
	//�����Ϸ���(0,1,0)
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


	particleSolve.onAdvanceTimeStep(0.009);

	for (int i = 0; i < n; ++i) {
		drawPoint(pos[i].x, pos[i].y);
	}

	Voronoi2 voronoiD;
	voronoiD.generateVoronoi(pos);

	drawVoronoi(voronoiD);


	//Ȼ��ǰ�󻺴潻�� 
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
	glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //����ʹ������ͶӰ
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
	//glClearColor(0, 0, 0, 1);
	glShadeModel(GL_FLAT);

	Array<Vector2D> pos(n);
	Vector2D temp1;
	for (int i = 0; i < n - 1; ++i) {
		temp1.x = random_double(0, 2);
		temp1.y = random_double(0, 2);
		pos[i] = temp1;
	}

	pos[n - 1] = Vector2D(1, 2);

	particleSolve.setData(n, pos, 2, 2);
	vector<ExplicitSurface2Ptr> surfaceSet;

	auto box1 = make_shared<Box2>(Vector2D(-0, -0), Vector2D(2, 2), true);
	Collider2 collider;
	collider.push(box1);

	particleSolve.setCollider(collider);



	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�zz����ʾʱ

	glutMainLoop();



	return 0;
}


