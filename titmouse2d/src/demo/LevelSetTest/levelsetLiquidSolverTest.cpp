//#include <iostream>
//#include <fstream>
//#include <cmath>
//#include <string>
//#include <vector>
//using namespace std;
//
//#include "../../ConstVar.h"
//
//#include <GL/glut.h>
//
//#include "../../Color3.hpp"
//#include "../../Vector2.hpp"
//#include "../../Eulerian/CellCenteredVectorGrid2.hpp"
//#include "../../Eulerian/FaceCenteredGrid2.h"
//#include "../../Eulerian/AdvectionSolver2.h"
//
//#include "../../Eulerian/SimpleLevelSetSolver2.h"
//
//#include "../../Eulerian/CellCenteredScalarGrid2.h"
//#include "../../Eulerian/VertexCenteredScalarGrid2.h"
//#include "../../Geometry/Sphere2.h"
//
//#include "../../Eulerian/MarchingCubes2.h"
//#include "../../Eulerian/SimpleLevelSetSolver2.h"
//
//#include "../../Eulerian/LevelSetLiquidSolver2.h"
//
//
//
//
//static void key(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//    case 27: //按ESC
//    case 'q': //按q键程序都会退出
//        exit(0);
//        break;
//    }
//
//    glutPostRedisplay();  //产生重绘回调
//}
//
//void drawPoint(double x, double y)
//{
//    //在后缓存绘制图形，就一个点
//    glPointSize(2.05f);//缺省是1
//    glBegin(GL_POINTS);
//    glColor3f(1, 128.0 / 255, 51.0 / 255);
//    glVertex3f((x - 1) * 10, (y - 1) * 10, 0);
//    glEnd();
//}
//
//void drawPoint(Vector2<double> pos, float size, Color3<float> color)
//{
//    glPointSize(size);
//    glBegin(GL_POINTS);
//    glColor3f(color.r / 255.0, color.g / 255.0, color.b / 255.0);
//    glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
//    glEnd();
//}
//
//void drawPoint(Vector2<double> pos, float size, Color3<double> color)
//{
//    glPointSize(size);
//    glBegin(GL_POINTS);
//    glColor3f(float(color.r) / 255.0, float(color.g) / 255.0, float(color.b) / 255.0);
//    glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
//    glEnd();
//}
//
//
//
//void drawLine(double x1, double y1, double x2, double y2) {
//
//    glLineWidth(1);//设置线段宽度
//    glBegin(GL_LINES);
//    glColor3f(1.0, 0.0, 0.0);
//    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //定点坐标范围
//    glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
//    glEnd();
//    glFlush();
//}
//
//
//void drawLine(double x1, double y1, double x2, double y2, Color3<float> color) {
//
//    glLineWidth(1);//设置线段宽度
//    glBegin(GL_LINES);
//    glColor3f(color.r / 255.0, color.g / 255.0, color.b / 255.0);
//    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //定点坐标范围
//    glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
//    glEnd();
//    glFlush();
//}
//
//
//Vector2<size_t> resolution(20, 20);
//Vector2<double> origin(0.0, 0.0);
//
//
//auto mc = MarchingCube2::builder()
//.withResolution(resolution)
//.withInitialValue(1.0)
//.withOrigin(0.0, 0.0)
//.makeShared();
//
//
//SimpleLevelSetSolver2 levelsetSolver;
//
//
//auto levelsetLiquidSolver = LevelSetLiquidSolver2::builder()
//.withResolution(resolution)
//.withOrigin(Vector2<double>())
//.makeShared();
//
//
//Vector2<double> centers(0.7, 1.5);
//double rs = 0.5;
//
//auto sphere1 = Sphere2(centers, rs, resolution, origin, 0.0);
//
//auto velocity = make_shared<FaceCenteredGrid2>(resolution, origin, Vector2<double>(0, 0));
//
//auto advectionSolver = make_shared<AdvectionSolver2>();
//
//static void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glLoadIdentity();
//    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//
//    levelsetLiquidSolver->onAdvanceTimeStep(0.02);
//    
//
//    vector<LineSegment> lineSet;
//    mc->getLineSegmentSet(lineSet, levelsetLiquidSolver->sdf());
//
//
//    for (auto i = lineSet.begin(); i != lineSet.end(); ++i) {
//        auto start = i->start;
//        auto end = i->End;
//        drawLine(start.x, start.y, end.x, end.y);
//    }
//
//    auto sdfSize = levelsetLiquidSolver->sdf()->dataSize();
//    for (int i = 0; i < sdfSize.x; ++i) {
//        for (int j = 0; j < sdfSize.y; ++j) {
//            auto posFunc = levelsetLiquidSolver->sdf()->dataPosition();
//            auto pos = posFunc(i, j);
//            if (levelsetLiquidSolver->sdf()->lookAt(i, j) <= 0) {
//                drawPoint(pos, 3.2f, Color3<float>(255, 0, 0));
//            }
//        }
//    }
//
//
//    glutSwapBuffers();
//
//}
//
//static void idle(void)
//{
//
//    glutPostRedisplay();
//
//}
//
//static void resize(int width, int height)
//{
//    const float ar = (float)width / (float)height;
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    double ratio = 0.1;
//    glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //这里使用正视投影
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
//
//
//int main(int argc, char** argv)
//{
//
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//    glutInitWindowSize(200, 200);
//    glutInitWindowPosition(0, 0);
//    glutCreateWindow("titmouse2d");
//
//    glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
//    glShadeModel(GL_FLAT);
//
//
//
//
//
//
//
//    //设置一个旋转的速度场
//    auto velSizeU = velocity->uSize();
//
//    for (int i = 0; i < velSizeU.x; ++i) {
//        for (int j = 0; j < velSizeU.y; ++j) {
//            auto posFunc = velocity->uPosition();
//            auto pos = posFunc(i, j);
//            auto tempVec = (pos - centers).getNormalize();
//            velocity->u(i, j) = tempVec.x * cos(kPiD / 2) - tempVec.y * sin(kPiD / 2);
//        }
//    }
//
//    auto velSizeV = velocity->vSize();
//
//    for (int i = 0; i < velSizeV.x; ++i) {
//        for (int j = 0; j < velSizeV.y; ++j) {
//            auto posFunc = velocity->vPosition();
//            auto pos = posFunc(i, j);
//            auto tempVec = (pos - centers).getNormalize();
//            velocity->v(i, j) = tempVec.x * sin(kPiD / 2) + tempVec.y * cos(kPiD / 2);
//        }
//    }
//
//
//    levelsetLiquidSolver->setFluidSdf(*sphere1.sdf());
//
//    glutKeyboardFunc(key);       //键盘按下去时
//    glutIdleFunc(idle);          //空闲时
//    glutReshapeFunc(resize);     //改变窗口大小时
//    glutDisplayFunc(display);    //绘制窗zz口显示时
//
//    glutMainLoop();
//
//
//
//
//
// // //这里是写入文件
// ////记得重新算的时候要删掉 原来的文件夹
// //int frame = 1000;
// ////auto num = flipSolver->particleSystemData()->numberOfParticles();
// ////auto position = flipSolver->particleSystemData()->positions();
//
// //string outfilename = "1";
//
// //int num = 0;
//
// //system("mkdir LevelSet2");
//
// //for (int i = 0; i < frame; i+= 1) {
// //   
// //    ofstream out("../titmouse2d/LevelSet2/"+outfilename + ".txt", ios::app);
//
// //    auto sdfSize = levelsetLiquidSolver->sdf()->dataSize();
// //    for (int i = 0; i < sdfSize.x; ++i) {
// //        for (int j = 0; j < sdfSize.y; ++j) {
// //            auto posFunc = levelsetLiquidSolver->sdf()->dataPosition();
// //            auto pos = posFunc(i, j);
// //            if (levelsetLiquidSolver->sdf()->lookAt(i, j) <= 0) {
// //                ++num;
// //                //drawPoint(pos, 3.2f, Color3<float>(255, 0, 0));
// //            }
// //        }
// //    }
//
//
// //    //然后，再写入像素数据
// //    for (int i = 0; i < sdfSize.x; ++i) {
// //        for (int j = 0; j < sdfSize.y; ++j) {
// //            auto posFunc = levelsetLiquidSolver->sdf()->dataPosition();
// //            auto pos = posFunc(i, j);
// //            if (levelsetLiquidSolver->sdf()->lookAt(i, j) <= 0) {
// //                out << pos.x << "," << pos.y << endl;
// //            }
// //        }
// //    }
//
// //    levelsetLiquidSolver->onAdvanceTimeStep(0.02);
// //    auto temp1 = std::atoi(outfilename.c_str());
// //    temp1++;
// //    outfilename = std::to_string(temp1);
// //    
// //}
//
//
//
//    return 0;
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
