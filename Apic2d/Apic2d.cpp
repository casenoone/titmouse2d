﻿//#include <iostream>
//#include <fstream>
//#include <cmath>
//#include <string>
//
//
//#include "../titmouse2d/src/Lagrangian/ParticleSystemSolver2.h"
//#include "../titmouse2d/src/Geometry/Box2.h"
//#include "../titmouse2d/src/Geometry/Plane2.h"
//
//#include "../titmouse2d/src/Hybrid/ApicSolver2.h"
//#include "../titmouse2d/src/Geometry/Heart2.h"
//
//#include <GL/glut.h>
//#include <cmath>
//
//const float SCREEN_SIZE = 400;
//const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;
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
//	glPointSize(2.5f);//缺省是1
//	glBegin(GL_POINTS);
//	glColor3f(1, 128.0 / 255, 51.0 / 255);
//	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
//	glEnd();
//}
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
//void drawColliders(const std::vector<ExplicitSurface2Ptr>& surfaceSet) {
//	for (auto i = surfaceSet.begin(); i != surfaceSet.end(); ++i) {
//		auto surface = (*i)->_data;
//		for (auto j = surface.begin(); j != surface.end(); ++j) {
//			auto start = j->start;
//			auto end = j->end;
//
//			drawLine(start.x, start.y, end.x, end.y);
//		}
//	}
//}
//
//
//auto apicSolver = ApicSolver2::builder()
//.withOrigin(Vector2D(0.0, 0.0))
//.withResolution(Vector2I(20, 20))
//.makeShared();
//
//
//double dt = 0.008;
//Collider2 collider;
//
//std::vector<ExplicitSurface2Ptr> surfaceSet;
//
//
//
//static void display(void)
//{
//
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	glLoadIdentity();
//	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//
//
//	auto num = apicSolver->particleSystemData()->numberOfParticles();
//	auto& pos = apicSolver->particleSystemData()->positions();
//	for (int i = 0; i < num; ++i) {
//		drawPoint(pos[i].x, pos[i].y);
//	}
//
//
//
//	apicSolver->onAdvanceTimeStep(dt);
//	drawColliders(surfaceSet);
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
//
//	int numberOfParticles = 0;
//	int resolutionX = 20;
//	int resolutionY = 20;
//	std::vector <Vector2D> temp1;
//	for (int i = 0; i < numberOfParticles; ++i) {
//		auto x = rand() / double(RAND_MAX) + 0.3;
//		auto y = rand() / double(RAND_MAX) + 0.95;
//		Vector2D temp(x, y);
//		temp1.push_back(temp);
//	}
//
//
//	Heart2 heart(Vector2D(1, 1), 0.1, Vector2I(100, 100), Vector2D::zero(), 0);
//	numberOfParticles = 0;
//	auto& heart_sdf = heart.sdf();
//	auto heart_res = heart_sdf->resolution();
//	for (int i = 0; i < heart_res.x; ++i) {
//		for (int j = 0; j < heart_res.y; ++j) {
//			if (heart_sdf->lookAt(i, j) < 0) {
//				auto cell_center = heart_sdf->cellCenterPosition()(i, j);
//				temp1.push_back(cell_center);
//				numberOfParticles++;
//			}
//		}
//	}
//
//
//	Array<Vector2D> pos(temp1);
//
//
//	Box2Ptr box1 = std::make_shared<Box2>(Vector2D(-0.01, -0.01), Vector2D(2.1, 2.1), true);
//	Box2Ptr box2 = std::make_shared<Box2>(Vector2D(0.1, 0.1), Vector2D(1.9, 1.9), true);
//	Box2Ptr box3 = std::make_shared<Box2>(Vector2D(0.6, 0.6), Vector2D(1.0, 0.7), false);
//	Plane2Ptr plane1 = std::make_shared<Plane2>(Vector2D(0.7, 0.8), Vector2D(1.0, 0.8), false);
//
//	surfaceSet.push_back(box1);
//	surfaceSet.push_back(box3);
//
//	collider.push(box1);
//	collider.push(box3);
//
//	apicSolver->setCollider(collider);
//	apicSolver->setData(numberOfParticles, pos, resolutionX, resolutionY);
//
//
//
//	glutKeyboardFunc(key);       //键盘按下去时
//	glutIdleFunc(idle);          //空闲时
//	glutReshapeFunc(resize);     //改变窗口大小时
//	glutDisplayFunc(display);    //绘制窗口显示时
//
//	glutMainLoop();
//
//
//
//
//
//	//这里是写入文件
////记得重新算的时候要删掉 原来的文件夹
//	int frame = 10000;
//	auto num = apicSolver->particleSystemData()->numberOfParticles();
//	auto position = apicSolver->particleSystemData()->positions();
//
//
//	int interval = 1;
//
//	std::string outfilename = "1";
//
//	//system("mkdir FlipData3");
//
//	for (int i = 0; i < frame; i += 1) {
//
//		std::ofstream out("E:\\zhangjian\\solve_data\\1\\" + outfilename + ".txt", std::ios::app);
//
//		for (int n = 0; n < num; ++n) {
//			auto x = position[n].x;
//			auto y = position[n].y;
//			out << x << "," << y << std::endl;
//		}
//		apicSolver->onAdvanceTimeStep(dt);
//		auto temp1 = std::atoi(outfilename.c_str());
//		temp1++;
//		outfilename = std::to_string(temp1);
//		std::cout << "当前解算到第" << temp1 << "帧" << std::endl;
//	}
//
//
//
//
//	return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
