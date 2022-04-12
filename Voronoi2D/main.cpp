#include <iostream>
using namespace std;

#include <cmath>


#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "Voronoi2.h"

#include <omp.h>




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
	glPointSize(2.05f);//ȱʡ��1
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
		//cout << edges[i]->start.x << endl;
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

Voronoi2 voronoiD;
static void display(void)
{




	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




	//����MODELVIEW��������Ϊ��λ���ٳ��Ϲ۲���󣬼���z������100���� ��0��0��0��
	//�����Ϸ���(0,1,0)
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

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

	voronoiD.generateVoronoi(500, 1, 1);

	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�zz����ʾʱ

	glutMainLoop();



	return 0;
}


