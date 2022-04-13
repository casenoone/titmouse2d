//#include <iostream>
//using namespace std;
//
//#include <fstream>
//#include <cmath>
//#include <string>
//#include <vector>
//
//#include "../titmouse2d/src/Geometry/Sphere2.h"
//#include "FoamVortexSolver.h"
//#include "../titmouse2d/src/SparseMatrix.hpp"
//#include "../titmouse2d/src/Eulerian/MarchingCubes2.h"
//#include "../titmouse2d/src/Geometry/RegularPolygon.h"
//#include <GL/glut.h>
//
//#include <windows.h>
//#include <Eigen/Dense>
//int sim_step = 0;
//
//const float SCREEN_SIZE = 600;
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
//void drawPoint(double x, double y, double color_x, double color_y, double color_z)
//{
//	//在后缓存绘制图形，就一个点
//	glPointSize(4.05f);//缺省是1
//	glBegin(GL_POINTS);
//	glColor3f(color_x / 255, color_y / 255, color_z / 255);
//	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
//	glEnd();
//}
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
//auto sphere1 = make_shared<Sphere2>(center1, r1, resolution);
//
//auto explicitSphere1 = sphere1->transformToExplicitSurface();
//
//auto vpSolver = make_shared<FoamVortexSolver>();
//
//auto sphereBox = sphere1->boundingBox();
//
//double movingCoffe = 0.3;
//
//Vector2I movingGridRes(10, 10);
//BoundingBox2 movingGridDomain(sphereBox.lowerCorner - Vector2D(movingCoffe, movingCoffe),
//	sphereBox.upperCorner + Vector2D(movingCoffe, movingCoffe));
//
//
//double dt = 0.008;
//
//RegularPolygonPtr obj1 = make_shared<RegularPolygon>(21, Vector2D(0.1, 1), 0.1);
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
//	obj1->velocity = Vector2D(1, 0.0);
//	obj1->updatePosition(dt);
//
//	vpSolver->onAdvanceTimeStep(dt);
//	sim_step++;
//	int numberOfParticles = vpSolver->foamVortexData()->numberOfParticles();
//
//	for (int i = 0; i < numberOfParticles; ++i) {
//
//		auto pos = vpSolver->foamVortexData()->positions();
//		//drawPoint(pos[i].x, pos[i].y, 255, 0, 0);
//	}
//
//	//可视化tracer粒子
//	auto tracer_pos = vpSolver->foamVortexData()->tracePosition;
//	int tracer_n = tracer_pos.dataSize();
//
//	for (int i = 0; i < tracer_n; ++i) {
//		if ((tracer_pos[i] - obj1->center()).getLength() > obj1->r())
//			drawPoint(tracer_pos[i].x, tracer_pos[i].y);
//	}
//
//	int m = 0;
//	for (auto i = obj1->_data.begin(); i != obj1->_data.end(); ++i) {
//		auto start = i->start;
//		auto end = i->end;
//		drawLine(start.x, start.y, end.x, end.y);
//
//		////可视化法线
//		//auto midPoint = obj1->midPoint(m++);
//		//auto normalEnd = midPoint + 0.2 * i->normal;
//		//drawLine(midPoint.x, midPoint.y, normalEnd.x, normalEnd.y);
//	}
//
//
//
//	/*auto movingSize = vpSolver->foamVortexData()->movingGrid->uSize();
//	for (int i = 0; i < movingSize.x; ++i) {
//		for (int j = 0; j < movingSize.y; ++j) {
//			auto posfunc = vpSolver->foamVortexData()->movingGrid->uPosition();
//			drawPoint(posfunc(i, j).x, posfunc(i, j).y);
//		}
//	}*/
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
//	vpSolver->setMovingGrid(movingGridRes, movingGridDomain);
//	vpSolver->emitTracerParticles();
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
//	//glutMainLoop();
//
//
//		//这里是写入文件
//	//记得重新算的时候要删掉 原来的文件夹
//	int frame = 100000;
//
//	auto position = vpSolver->foamVortexData()->positions();
//
//
//	int interval = 1;
//
//	string outfilename = "1";
//
//	system("mkdir FoamTest12");
//
//	for (int i = 0; i < frame; i += 1) {
//
//		ofstream out("E:\\zhangjian\\paper_and_project\\titmouse2d\\OpenGL\\FoamTest12\\" + outfilename + ".txt", ios::app);
//		auto num = vpSolver->foamVortexData()->numberOfParticles();
//		auto tracer_num = vpSolver->foamVortexData()->tracePosition.dataSize();
//
//
//
//		obj1->velocity = Vector2D(1, 0.0);
//		obj1->updatePosition(dt);
//
//		for (int n = 0; n < tracer_num; ++n) {
//			auto x = vpSolver->foamVortexData()->tracePosition[n].x;
//			auto y = vpSolver->foamVortexData()->tracePosition[n].y;
//			if (x < 2 && y < 2 && x >= 0 && y >= 0)
//				out << x << "," << y << endl;
//		}
//		vpSolver->onAdvanceTimeStep(dt);
//		sim_step++;
//		auto temp1 = std::atoi(outfilename.c_str());
//		temp1++;
//		outfilename = std::to_string(temp1);
//		cout << "当前计算到第" << sim_step << "步,系统中粒子数：" << num + tracer_num << endl;
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
