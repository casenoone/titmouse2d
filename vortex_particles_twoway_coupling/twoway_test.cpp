//#include <iostream>
//
//#include <fstream>
//#include <cmath>
//#include <string>
//#include <vector>
//
//#include "../titmouse2d/src/Geometry/Sphere2.h"
//#include "VortexParticleSolver2.h"
//#include "../titmouse2d/src/SparseMatrix.hpp"
//#include "../titmouse2d/src/Eulerian/MarchingCubes2.h"
//#include "../titmouse2d/src/Geometry/RegularPolygon.h"
//#include "../titmouse2d/src/Eulerian/CellCenteredScalarGrid2.h"
//#include "../titmouse2d/src/random.h"
//
//#include <GL/glut.h>
//
//#include <windows.h>
//#include <Eigen/Dense>
//int sim_step = 0;
//
//const float SCREEN_SIZE = 400;
//const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;
//
//static void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
//
//static void key(unsigned char key, int x, int y)
//{
//	switch (key)
//	{
//	case 27: //按ESC
//	case 'q': //按q键程序都会退出
//		exit(0);
//		break;
//	}
//
//	glutPostRedisplay();  //产生重绘回调
//}
//
//void drawPoint(double x, double y)
//{
//	//在后缓存绘制图形，就一个点
//	glPointSize(3.05f);//缺省是1
//	glBegin(GL_POINTS);
//	glColor3f(1, 128.0 / 255, 51.0 / 255);
//	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
//	glEnd();
//}
//
//
//
//
//
//void drawLine(double x1, double y1, double x2, double y2) {
//
//	glLineWidth(1);//设置线段宽度
//	glBegin(GL_LINES);
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //定点坐标范围
//	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
//	glEnd();
//	glFlush();
//}
//
//
//
//
//Vector2I resolution(13, 13);
//Vector2D origin(0.0, 0.0);
//
//Vector2D center1(1.0, 1.1);
//double r1 = 0.4;
//
//
//
//
//auto vpSolver = std::make_shared<FoamVortexSolver>();
//
//double dt = 0.01;
//
////double dt = 0.0008;
//RegularPolygonPtr obj1 = std::make_shared<RegularPolygon>(10, Vector2D(1.0, 1.0), 0.01);
//
//
//
//
//static void display(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glLoadIdentity();
//	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//	vpSolver->onAdvanceTimeStep(dt);
//	sim_step++;
//	auto newF = vpSolver->computeTwoWayForce(dt);
//	obj1->velocity += newF * dt;
//	obj1->updatePosition(dt);
//
//	/***********以下临时测验bubble_panelSet**********/
//	//可视化泡沫
//	auto& bubble_circle = vpSolver->foamVortexData()->bubble_panelset;
//	auto bubble_circle_n = bubble_circle.dataSize();
//	for (int i = 0; i < bubble_circle_n; ++i) {
//
//		for (auto j = bubble_circle[i]->_data.begin(); j != bubble_circle[i]->_data.end(); ++j) {
//			auto start = j->start;
//			auto end = j->end;
//			drawLine(start.x, start.y, end.x, end.y);
//		}
//
//		auto newF = vpSolver->computeTwoWayForce(i, dt);
//		bubble_circle[i]->velocity += newF * dt;
//		bubble_circle[i]->updatePosition(dt);
//	}
//	/***********以上临时测验bubble_panelSet**********/
//
//
//
//	//可视化涡粒子
//	auto vor_pos = vpSolver->foamVortexData()->vortexPosition;
//	int vor_n = vor_pos.dataSize();
//	for (int i = 0; i < vor_n; ++i) {
//		drawPoint(vor_pos[i].x, vor_pos[i].y);
//
//	}
//
//	//可视化tracer粒子
//	auto tracer_pos = vpSolver->foamVortexData()->tracePosition;
//	int tracer_n = tracer_pos.dataSize();
//
//	for (int i = 0; i < tracer_n; ++i) {
//		drawPoint(tracer_pos[i].x, tracer_pos[i].y);
//	}
//
//
//
//	int m = 0;
//	for (auto i = obj1->_data.begin(); i != obj1->_data.end(); ++i) {
//		auto start = i->start;
//		auto end = i->end;
//		drawLine(start.x, start.y, end.x, end.y);
//	}
//
//
//
//	glutSwapBuffers();
//
//}
//
//static void idle(void)
//{
//
//	glutPostRedisplay();
//
//}
//
//static void resize(int width, int height)
//{
//	const float ar = (float)width / (float)height;
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	double ratio = 0.1;
//	glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //这里使用正视投影
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//
//int main(int argc, char** argv)
//{
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("titmouse2d");
//
//	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
//	glShadeModel(GL_FLAT);
//
//	//vpSolver->setPanels(explicitSphere1);
//	vpSolver->setPanels(obj1);
//	//vpSolver->emitParticles();
//	vpSolver->emitVortexRing();
//
//	obj1->velocity = Vector2D(0, .0);
//
//
//
//
//
//	double temp_r = 0.01;
//	Array<Vector2D> this_pos;
//	Array<double> this_radius;
//	Vector2D temp1;
//
//	int bubble_num = 0;
//	auto grid = CellCenteredScalarGrid2::builder()
//		.withOrigin(0, 0)
//		.withResolution(15, 15)
//		.makeShared();
//
//	Vector2D tempC(1, 1);
//	for (int i = 0; i < grid->resolution().x; ++i) {
//		for (int j = 0; j < grid->resolution().y; ++j) {
//			auto pos = (grid->dataPosition())(i, j);
//			if (pos.dis(tempC) < 0.2) {
//				//pos.x += random_double(-0.01, 0.01);
//				//pos.y += random_double(-0.01, 0.01);
//
//				this_pos.push(pos);
//				//temp_r = random_double(0.01, 0.03);
//				this_radius.push(temp_r);
//				//bubbleSolver._bubbleData->particleRadius.push(temp_r);
//				bubble_num++;
//			}
//		}
//	}
//
//	//vpSolver->setData(bubble_num, this_pos, 2, 2);
//
//	vpSolver->generatePanelSet(this_pos, this_radius);
//
//
//	UINT timerId = 1;
//	MSG msg;
//	SetTimer(NULL, timerId, 1, TimerProc);
//
//
//
//
//	glutKeyboardFunc(key);       //键盘按下去时
//	glutIdleFunc(idle);          //空闲时
//	glutReshapeFunc(resize);     //改变窗口大小时
//	glutDisplayFunc(display);    //绘制窗zz口显示时
//
//	glutMainLoop();
//
//
//	//这里是写入文件
////记得重新算的时候要删掉 原来的文件夹
//	int frame = 100000;
//
//	auto position = vpSolver->foamVortexData()->positions();
//
//
//	int interval = 1;
//
//	std::string outfilename = "1";
//
//	system("mkdir FoamTest9");
//
//	for (int i = 0; i < frame; i += 1) {
//
//		std::ofstream out("E:\\zhangjian\\paper_and_project\\titmouse2d\\OpenGL\\FoamTest9\\" + outfilename + ".txt", std::ios::app);
//		auto num = vpSolver->foamVortexData()->numberOfParticles();
//		auto tracer_num = vpSolver->foamVortexData()->tracePosition.dataSize();
//		for (int n = 0; n < num; ++n) {
//			auto x = position[n].x;
//			auto y = position[n].y;
//			if (x < 2 && y < 2)
//				out << x << "," << y << std::endl;
//		}
//		vpSolver->onAdvanceTimeStep(dt);
//		sim_step++;
//		auto temp1 = std::atoi(outfilename.c_str());
//		temp1++;
//		outfilename = std::to_string(temp1);
//		std::cout << "当前计算到第" << sim_step << "步,系统中粒子数：" << num + tracer_num << std::endl;
//
//	}
//
//
//
//	return 0;
//}
//
//
//void CALLBACK TimerProc(HWND hwnd, UINT Msg, UINT idEvent, DWORD dwTime)
//{
//
//
//
//}
//
//
//
