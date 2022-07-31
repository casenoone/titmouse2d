#include <iostream>

#include <fstream>
#include <cmath>
#include <string>
#include <vector>

#include "../titmouse2d/src/Geometry/Sphere2.h"
#include "FoamVortexSolver2.h"
#include "../titmouse2d/src/SparseMatrix.hpp"
#include "../titmouse2d/src/Eulerian/MarchingCubes2.h"
#include "../titmouse2d/src/Geometry/RegularPolygon.h"
#include "../titmouse2d/src/Geometry/Box2.h"
#include "../titmouse2d/src/mesh/Plyout.h"
#include <GL/glut.h>

#include <windows.h>
#include <Eigen/Dense>
int sim_step = 0;

const float SCREEN_SIZE = 600;
const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;

static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

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
	glPointSize(3.05f);//ȱʡ��1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
	glEnd();
}


void drawPoint(double x, double y, double color_x, double color_y, double color_z)
{
	//�ں󻺴����ͼ�Σ���һ����
	glPointSize(4.05f);//ȱʡ��1
	glBegin(GL_POINTS);
	glColor3f(color_x / 255, color_y / 255, color_z / 255);
	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
	glEnd();
}


void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(1);//�����߶ο��
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //�������귶Χ
	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
	glEnd();
	glFlush();
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





Vector2D center1(1.0, 1.1);
double r1 = 0.4;

auto res = Vector2I(70, 70);
auto grid_x = 2 / 70.0;

auto vpSolver = std::make_shared<FoamVortexSolver>(res, Vector2D(grid_x, grid_x));
RegularPolygonPtr obj1 = std::make_shared<RegularPolygon>(21, Vector2D(0.1, 1), 0.06);

double dt = 0.006;
int bubble_num = 0;

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
	vpSolver->onAdvanceTimeStep(dt);

	obj1->updatePosition(dt);

	///***********������ʱ����bubble_panelSet**********/
	//auto& bubble_circle = vpSolver->foamVortexData()->bubble_panelset;
	//auto bubble_circle_n = bubble_circle.dataSize();
	//for (int i = 0; i < bubble_circle_n; ++i) {

	//	for (auto j = bubble_circle[i]->_data.begin(); j != bubble_circle[i]->_data.end(); ++j) {
	//		auto start = j->start;
	//		auto end = j->end;
	//		drawLine(start.x, start.y, end.x, end.y);
	//	}
	//}
	///***********������ʱ����bubble_panelSet**********/

	auto& bubble_pos = vpSolver->foamVortexData()->positions();
	//���ӻ�����
	static int fileNum = 1;
	std::string	name = std::to_string(fileNum);
	fileNum++;
	std::string path1 = "E:\\zhangjian\\solve_data\\all\\bubble\\";
	Plyout writer1(path1, name, bubble_pos.dataSize(), "r");
	for (int i = 0; i < bubble_pos.dataSize(); ++i) {
		drawCircle(bubble_pos[i], vpSolver->foamVortexData()->radius, 50);
		auto r = 0.03;
		//writer1.write_in_ply(bubble_pos[i].x, 0, bubble_pos[i].y, r);
	}


	vpSolver->setShallowWaveMovingBoundary(obj1->center(), obj1->r());
	sim_step++;
	int n = vpSolver->foamVortexData()->vortexPosition.dataSize();

	for (int i = 0; i < n; ++i) {

		auto pos = vpSolver->foamVortexData()->vortexPosition;
		drawPoint(pos[i].x, pos[i].y, 10, 0, 0);
	}

	//���ӻ�tracer����
	auto tracer_pos = vpSolver->foamVortexData()->tracePosition;
	int tracer_n = tracer_pos.dataSize();

	for (int i = 0; i < tracer_n; ++i) {
		if ((tracer_pos[i] - obj1->center()).getLength() > obj1->r())
			drawPoint(tracer_pos[i].x, tracer_pos[i].y);
	}

	//���ӻ��ƶ��߽�
	for (auto i = obj1->_data.begin(); i != obj1->_data.end(); ++i) {
		auto start = i->start;
		auto end = i->end;
		drawLine(start.x, start.y, end.x, end.y);
	}



	glutSwapBuffers();
}

static void idle(void) {
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
	glShadeModel(GL_FLAT);

	vpSolver->setMovingBoudnary(obj1);
	vpSolver->emitTracerParticles();
	auto box1 = std::make_shared<Box2>(Vector2D(0, 0), Vector2D(2, 2), true);

	vpSolver->setStaticBoudnary(box1);


	/**********������������**********/
	Array<Vector2D> this_pos;
	Vector2D temp1;

	auto grid = CellCenteredScalarGrid2::builder()
		.withOrigin(0, 0)
		.withResolution(110, 110)
		.makeShared();
	double temp_r = 0.01;// grid->gridSpacing().x * 0.5;

	//Vector2D tempC(1.0, 1.0);
	Vector2D tempC(0.6, 1.0);
	for (int i = 0; i < grid->resolution().x; ++i) {
		for (int j = 0; j < grid->resolution().y; ++j) {
			auto pos = (grid->dataPosition())(i, j);
			if (pos.dis(tempC) < 0.4) {
				//pos.x += random_double(-0.02, 0.02);
				//pos.y += random_double(-0.02, 0.02);
				this_pos.push(pos);
				//temp_r = random_double(0.01, 0.03);
				bubble_num++;
			}
		}
	}

	vpSolver->setData(bubble_num, this_pos, 20, 20);
	//vpSolver->emitVortexRing();

	Collider2 collider;
	collider.push(box1);

	vpSolver->setCollider(collider);
	/**********������������**********/

	obj1->velocity = Vector2D(3, 0.0);

	vpSolver->generatePanelSet(this_pos);

	vpSolver->foamVortexData()->restLen = grid->gridSpacing().x;
	vpSolver->foamVortexData()->radius = grid->gridSpacing().x / 2;
	UINT timerId = 1;
	MSG msg;
	SetTimer(NULL, timerId, 1, TimerProc);


	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�����ʾʱ

	//glutMainLoop();




































	//int frame = 100000;
	//auto position = vpSolver->foamVortexData()->positions();
	//auto waterdata = vpSolver->_shallowWaveSolver->shallowWaveData();
	//auto water_num = waterdata->resolution().x * waterdata->resolution().x;
	//auto bubble_num = vpSolver->foamVortexData()->numberOfParticles();
	//for (int i = 0; i < frame; i += 1) {

	//	auto tracer_num = vpSolver->foamVortexData()->tracePosition.dataSize();
	//	obj1->velocity = Vector2D(2, 0.0);
	//	obj1->updatePosition(dt);
	//	vpSolver->setShallowWaveMovingBoundary(obj1->center(), obj1->r());

	//	static int fileNum = 1;
	//	std::string	name = std::to_string(fileNum);
	//	fileNum++;
	//	std::string path1 = "E:\\zhangjian\\solve_data\\all_1\\boundary\\";
	//	std::string path2 = "E:\\zhangjian\\solve_data\\all_1\\thinfoam\\";
	//	std::string path3 = "E:\\zhangjian\\solve_data\\all_1\\water\\";
	//	std::string path4 = "E:\\zhangjian\\solve_data\\all_1\\bubble\\";

	//	Plyout writer1(path1, name, 1);
	//	Plyout writer2(path2, name, tracer_num + 4);
	//	Plyout writer3(path3, name, water_num);
	//	Plyout writer4(path4, name, bubble_num + 4);


	//	//д���ƶ��߽������
	//	writer1.write_in_ply(obj1->center().x, 0, obj1->center().y);

	//	//д��thinfoam����
	//	for (int n = 0; n < tracer_num; ++n) {
	//		auto x = vpSolver->foamVortexData()->tracePosition[n].x;
	//		auto y = vpSolver->foamVortexData()->tracePosition[n].y;
	//		if (x < 2 && y < 2 && x >= 0 && y >= 0) {
	//			auto height = waterdata->height->sample(vpSolver->foamVortexData()->tracePosition[n]);
	//			writer2.write_in_ply(x, height, y);
	//		}
	//	}


	//	//Ϊ�˱�֤thinfoamģ����waterģ�Ͷ��룬׷���ĸ���
	//	writer2.write_in_ply(0, 0, 0);
	//	writer2.write_in_ply(2, 0, 2);
	//	writer2.write_in_ply(2, 0, 0);
	//	writer2.write_in_ply(0, 0, 2);

	//	//д��water����
	//	for (int q1 = 0; q1 < waterdata->resolution().x; ++q1) {
	//		for (int q2 = 0; q2 < waterdata->resolution().y; ++q2) {
	//			auto posFunc = waterdata->height->dataPosition();
	//			auto pos = posFunc(q1, q2);
	//			auto height = waterdata->height->lookAt(q1, q2);
	//			writer3.write_in_ply(pos.x, height, pos.y);
	//		}
	//	}


	//	//д��bubble����
	//	for (int n = 0; n < bubble_num; ++n) {
	//		auto height = waterdata->height->sample(position[n]);
	//		writer4.write_in_ply(position[n].x, height, position[n].y);
	//	}

	//	//Ϊ�˱�֤bubbleģ����waterģ�Ͷ��룬׷���ĸ���
	//	writer4.write_in_ply(0, 0, 0);
	//	writer4.write_in_ply(2, 0, 2);
	//	writer4.write_in_ply(2, 0, 0);
	//	writer4.write_in_ply(0, 0, 2);

	//	vpSolver->onAdvanceTimeStep(dt);
	//	auto vortex_num = vpSolver->foamVortexData()->vortexPosition.dataSize();
	//	std::cout << "��ǰ���㵽�ڣ�" << i << "��������������" << vortex_num << std::endl;

	//}








		//������д���ļ�
	//�ǵ��������ʱ��Ҫɾ�� ԭ�����ļ���
	int frame = 100000000;

	auto position = vpSolver->foamVortexData()->positions();
	//auto position = vpSolver->foamVortexData()->tracePosition;
	auto num = position.dataSize();

	int interval = 1;

	std::string outfilename = "1";

	for (int i = 0; i < frame; i += 1) {

		std::ofstream out("E:\\zhangjian\\solve_data\\0022\\" + outfilename + ".txt", std::ios::app);

		for (int n = 0; n < num; ++n) {
			auto x = position[n].x;
			auto y = position[n].y;
			out << x << "," << y << std::endl;
		}
		vpSolver->onAdvanceTimeStep(dt);
		obj1->updatePosition(dt);
		auto temp1 = std::atoi(outfilename.c_str());
		temp1++;
		outfilename = std::to_string(temp1);
		std::cout << "��ǰ���㵽" << i << "֡" << std::endl;
	}














	return 0;
}


void CALLBACK TimerProc(HWND hwnd, UINT Msg, UINT idEvent, DWORD dwTime)
{



}



