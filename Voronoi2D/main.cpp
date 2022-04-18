#include <iostream>

#include <cmath>

#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>

#include <GL/glut.h>

#include "../Voronoi2D/Voronoi2.h"
#include "bubbleSolver2.h"
#include "../titmouse2d/src/random.h"
#include "../titmouse2d/src/Geometry/Box2.h"
#include "../titmouse2d/src/Collider2.h"
#include "../titmouse2d/src/Eulerian/CellCenteredScalarGrid2.h"

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


void drawCircle(const Vector2D& center, double r, int res, float r_, float g_, float b_) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(r_, g_, b_);
		glVertex2f(x, y);
	}
	glEnd();
	/*glBegin(GL_LINE_LOOP);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(0, 0, 0);
		glVertex2f(x, y);
	}
	glEnd();*/
}



void drawCircle(const Vector2D& center, double r, int res) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(1, 128.0 / 255, 51.0 / 255);
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(0, 0, 0);
		glVertex2f(x, y);
	}
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


BubbleSolver2 bubbleSolver;
void drawVoronoi(const Voronoi2& voronoi, Array<Vector2D>& pos) {
	auto& edges = voronoi._data.edges;
	auto& sites = voronoi._data.sites;

	auto edgeLength = edges.size();
	for (int i = 0; i < edgeLength; ++i) {
		//drawLine(edges[i]->start.x, edges[i]->start.y,
		//	edges[i]->end.x, edges[i]->end.y);
		int index = edges[i]->leftIndex;
		//drawPoint(pos[index].x, pos[index].y);
		drawCircle(pos[index], bubbleSolver._bubbleData->particleRadius.lookAt(index), 50);
	}



}

int number = 3;
int w = 200;
int h = 200;
int sweepY = 0;

int n = 100;




auto& pos = bubbleSolver._bubbleData->positions();
auto& vortexPos = bubbleSolver._bubbleData->vortexPosition;
static void display(void)
{



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//����MODELVIEW��������Ϊ��λ���ٳ��Ϲ۲���󣬼���z������100���� ��0��0��0��
	//�����Ϸ���(0,1,0)
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


	Voronoi2 voronoiD;

	voronoiD.generateVoronoi(pos);

	drawVoronoi(voronoiD, pos);
	bubbleSolver.onAdvanceTimeStep(0.005, voronoiD);

	for (int i = 0; i < vortexPos.dataSize(); ++i) {
		drawCircle(vortexPos[i], 0.02, 4, 1, 1, 1);
	}

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



	double temp_r = 0.03;
	Array<Vector2D> this_pos;
	Vector2D temp1;

	n = 0;
	auto grid = CellCenteredScalarGrid2::builder()
		.withOrigin(0, 0)
		.withResolution(22, 22)
		.makeShared();

	Vector2D tempC(1, 1);
	for (int i = 0; i < grid->resolution().x; ++i) {
		for (int j = 0; j < grid->resolution().y; ++j) {
			auto pos = (grid->dataPosition())(i, j);
			if (pos.dis(tempC) < 0.4) {
				pos.x += random_double(-0.01, 0.01);
				pos.y += random_double(-0.01, 0.01);

				this_pos.push(pos);
				bubbleSolver._bubbleData->particleRadius.push(temp_r);
				n++;
			}
		}
	}

	std::cout << n << std::endl;
	bubbleSolver.setData(n, this_pos, 2, 2);
	std::vector<ExplicitSurface2Ptr> surfaceSet;

	auto box1 = std::make_shared<Box2>(Vector2D(-0, -0), Vector2D(2, 2), true);
	Collider2 collider;
	collider.push(box1);

	bubbleSolver.setCollider(collider);
	bubbleSolver.emitVortexRing();


	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�zz����ʾʱ

	glutMainLoop();


	//������д���ļ�
//�ǵ��������ʱ��Ҫɾ�� ԭ�����ļ���
	int frame = 100000;

	auto position = bubbleSolver._bubbleData->positions();
	auto num = bubbleSolver._bubbleData->numberOfParticles();


	int interval = 1;

	std::string outfilename = "1";

	system("mkdir FoamTest12");

	for (int i = 0; i < frame; i += 1) {

		std::ofstream out("E:\\zhangjian\\paper_and_project\\titmouse2d\\Voronoi2D\\FoamTest12\\" + outfilename + ".txt", std::ios::app);
		for (int n = 0; n < num; ++n) {
			auto x = position[n].x;
			auto y = position[n].y;
			if (x < 2 && y < 2)
				out << x << "," << y << std::endl;
		}
		Voronoi2 voronoiD;
		voronoiD.generateVoronoi(pos);
		bubbleSolver.onAdvanceTimeStep(0.005, voronoiD);
		auto temp1 = std::atoi(outfilename.c_str());
		temp1++;
		outfilename = std::to_string(temp1);
		std::cout << "��ǰ���㵽�ڣ�" << temp1 << "��" << std::endl;
	}








	return 0;
}


