//#include <iostream>
//#include <fstream>
//#include <cmath>
//#include <string>
//#include <GL/glut.h>
//#include <vector>
//#include "ConstrainedSolver2.h"
//#include "../titmouse2d/src/random.h"
//#include "../titmouse2d/src/Eulerian/CellCenteredScalarGrid2.h"
//#include "../titmouse2d/src/Geometry/Heart2.h"
//#include "../titmouse2d/src/Geometry/Box2.h"
//#include "../titmouse2d/src/Hybrid/ApicSolver2.h"
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
//	glPointSize(4.0f);//缺省是1
//	glBegin(GL_POINTS);
//	glColor3f(1, 128.0 / 255, 51.0 / 255);
//	glColor3f(1.0, 0.0, 0.0);
//
//	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
//	glEnd();
//}
//
//void drawLine(double x1, double y1, double x2, double y2) {
//
//	glLineWidth(1);//设置线段宽度
//	glBegin(GL_LINES);
//	glColor3f(1.0, 0.0, 0.0);
//	glColor3f(1, 128.0 / 255, 51.0 / 255);
//
//	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //定点坐标范围
//	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
//	glEnd();
//	glFlush();
//}
//
//
//std::shared_ptr<ConstrainedSolver2> constSolver;
//double dt = 0.01;
//int n;
//std::vector<ExplicitSurface2Ptr> surfaceSet;
//auto apicSolver = ApicSolver2::builder()
//.withOrigin(Vector2D(0.0, 0.0))
//.withResolution(Vector2I(70, 70))
//.makeShared();
//static void display(void)
//{
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//	/*************************以下程序主逻辑***************************/
//	constSolver->onAdvanceTimeStep(dt);
//
//	/*************************以上程序主逻辑***************************/
//	//std::cout << 888 << std::endl;
//	//绘制质点
//	for (int i = 0; i < n; ++i) {
//		auto& pos = constSolver->massSpringData->positions;
//		drawPoint(pos[i].x, pos[i].y);
//	}
//
//	//绘制约束
//	auto& edge = constSolver->massSpringData->edges;
//	auto edgeNum = edge.dataSize();
//	for (int i = 0; i < edgeNum; ++i) {
//		auto& pos = constSolver->massSpringData->positions;
//		auto pi = edge[i].i;
//		auto pj = edge[i].j;
//		drawLine(pos[pi].x, pos[pi].y, pos[pj].x, pos[pj].y);
//	}
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
//	double numH = 0.6;
//
//	Box2Ptr box1 = std::make_shared<Box2>(Vector2D(0.01, 0.01), Vector2D(2.1, 2.1), true);
//
//	Box2Ptr p1 = std::make_shared<Box2>(Vector2D(0.01 + 0.15, 0.01 + numH), Vector2D(0.3 + 0.15, 0.07 + numH), false);
//	Box2Ptr p2 = std::make_shared<Box2>(Vector2D(0.01 + 0.15, 0.01 + numH), Vector2D(0.08 + 0.15, 0.23 + numH), false);
//	Box2Ptr p3 = std::make_shared<Box2>(Vector2D(0.01 + 0.15, 0.16 + numH), Vector2D(0.3 + 0.15, 0.24 + numH), false);
//	Box2Ptr p4 = std::make_shared<Box2>(Vector2D(0.24 + 0.15, 0.17 + numH), Vector2D(0.31 + 0.15, 0.41 + numH), false);
//	Box2Ptr p5 = std::make_shared<Box2>(Vector2D(0.01 + 0.15, 0.33 + numH), Vector2D(0.3 + 0.15, 0.41 + numH), false);
//
//	Box2Ptr q1 = std::make_shared<Box2>(Vector2D(0.45 + 0.15, 0.01 + numH), Vector2D(0.52 + 0.15, 0.41 + numH), false);
//	Box2Ptr q2 = std::make_shared<Box2>(Vector2D(0.45 + 0.15, 0.01 + numH), Vector2D(0.75 + 0.15, 0.07 + numH), false);
//	Box2Ptr q3 = std::make_shared<Box2>(Vector2D(0.45 + 0.15, 0.34 + numH), Vector2D(0.75 + 0.15, 0.41 + numH), false);
//	Box2Ptr q4 = std::make_shared<Box2>(Vector2D(0.68 + 0.15, 0.01 + numH), Vector2D(0.75 + 0.15, 0.41 + numH), false);
//
//	Box2Ptr r1 = std::make_shared<Box2>(Vector2D(0.9 + 0.15, 0.01 + numH), Vector2D(1.2 + 0.15, 0.07 + numH), false);
//	Box2Ptr r2 = std::make_shared<Box2>(Vector2D(0.9 + 0.15, 0.01 + numH), Vector2D(0.98 + 0.15, 0.23 + numH), false);
//	Box2Ptr r3 = std::make_shared<Box2>(Vector2D(0.9 + 0.15, 0.16 + numH), Vector2D(1.2 + 0.15, 0.24 + numH), false);
//	Box2Ptr r4 = std::make_shared<Box2>(Vector2D(1.14 + 0.15, 0.17 + numH), Vector2D(1.21 + 0.15, 0.41 + numH), false);
//	Box2Ptr r5 = std::make_shared<Box2>(Vector2D(0.9 + 0.15, 0.33 + numH), Vector2D(1.2 + 0.15, 0.41 + numH), false);
//
//	Box2Ptr s1 = std::make_shared<Box2>(Vector2D(1.4 + 0.15, 0.01 + numH), Vector2D(1.7 + 0.15, 0.07 + numH), false);
//	Box2Ptr s2 = std::make_shared<Box2>(Vector2D(1.63 + 0.15, 0.01 + numH), Vector2D(1.70 + 0.15, 0.41 + numH), false);
//	Box2Ptr s3 = std::make_shared<Box2>(Vector2D(1.4 + 0.15, 0.34 + numH), Vector2D(1.7 + 0.15, 0.41 + numH), false);
//	Box2Ptr s4 = std::make_shared<Box2>(Vector2D(1.4 + 0.15, 0.17 + numH), Vector2D(1.7 + 0.15, 0.24 + numH), false);
//
//
//
//
//	surfaceSet.push_back(box1);
//	surfaceSet.push_back(p1);
//	surfaceSet.push_back(p2);
//	surfaceSet.push_back(p3);
//	surfaceSet.push_back(p4);
//	surfaceSet.push_back(p5);
//	surfaceSet.push_back(q1);
//	surfaceSet.push_back(q2);
//	surfaceSet.push_back(q3);
//	surfaceSet.push_back(q4);
//	surfaceSet.push_back(r1);
//	surfaceSet.push_back(r2);
//	surfaceSet.push_back(r3);
//	surfaceSet.push_back(r4);
//	surfaceSet.push_back(r5);
//	surfaceSet.push_back(s1);
//	surfaceSet.push_back(s2);
//	surfaceSet.push_back(s3);
//	surfaceSet.push_back(s4);
//
//
//	n = 0;
//	std::vector<Vector2D> pos(n);
//
//
//	auto spacing = apicSolver->gridSpacing();
//	auto res = apicSolver->resolution();
//	auto posFunc = apicSolver->velocity()->cellCenterPosition();
//
//
//	for (auto& sur : surfaceSet) {
//		auto son = std::dynamic_pointer_cast<Box2>(sur);
//		for (int i = 0; i < res.x; ++i) {
//			for (int j = 0; j < res.y; ++j) {
//
//				if (son->IsInSide(posFunc(i, j)) && son->fliped == false) {
//					for (int k = 0; k < 1; ++k) {
//						n++;
//						auto tempP = posFunc(i, j);
//						auto randX = random_double(tempP.x - spacing.x / 2, tempP.x + spacing.x / 2);
//						auto randY = random_double(tempP.y - spacing.x / 2, tempP.y + spacing.x / 2);
//
//						pos.push_back(Vector2D(randX, randY));
//					}
//
//
//				}
//			}
//		}
//
//
//	}
//	std::cout << n << std::endl;
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
//	/*************************以下程序主逻辑***************************/
//
//
//
//
//	/*Heart2 heart(Vector2D(1, 1), 0.1, Vector2I(30, 30), Vector2D::zero(), 0);
//	auto& heart_sdf = heart.sdf();
//	auto heart_res = heart_sdf->resolution();
//	for (int i = 0; i < heart_res.x; ++i) {
//		for (int j = 0; j < heart_res.y; ++j) {
//			if (heart_sdf->lookAt(i, j) < 0) {
//				auto cell_center = heart_sdf->cellCenterPosition()(i, j);
//				pos.push_back(cell_center);
//				n++;
//			}
//		}
//	}*/
//
//	constSolver = std::make_shared<ConstrainedSolver2>(pos);
//
//	/*************************以上程序主逻辑***************************/
//
//	glutKeyboardFunc(key);       //键盘按下去时
//	glutIdleFunc(idle);          //空闲时
//	glutReshapeFunc(resize);     //改变窗口大小时
//	glutDisplayFunc(display);    //绘制窗口显示时
//
//	glutMainLoop();
//
//	//这里是写入文件
////记得重新算的时候要删掉 原来的文件夹
//	int frame = 10000;
//	auto num = constSolver->massSpringData->numberOfPoint;
//	auto position = constSolver->massSpringData->positions;
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
//		std::ofstream out("E:\\zhangjian\\solve_data\\test520_1\\" + outfilename + ".txt", std::ios::app);
//
//		for (int n = 0; n < num; ++n) {
//			auto x = position[n].x;
//			auto y = position[n].y;
//			out << x << "," << y << std::endl;
//		}
//		constSolver->onAdvanceTimeStep(dt);
//		auto temp1 = std::atoi(outfilename.c_str());
//		temp1++;
//		outfilename = std::to_string(temp1);
//		std::cout << "当前解算到第" << temp1 << "帧" << std::endl;
//	}
//
//
//	return 0;
//}
