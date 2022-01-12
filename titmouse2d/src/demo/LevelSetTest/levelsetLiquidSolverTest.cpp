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
//    case 27: //��ESC
//    case 'q': //��q�����򶼻��˳�
//        exit(0);
//        break;
//    }
//
//    glutPostRedisplay();  //�����ػ�ص�
//}
//
//void drawPoint(double x, double y)
//{
//    //�ں󻺴����ͼ�Σ���һ����
//    glPointSize(2.05f);//ȱʡ��1
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
//    glLineWidth(1);//�����߶ο��
//    glBegin(GL_LINES);
//    glColor3f(1.0, 0.0, 0.0);
//    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //�������귶Χ
//    glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
//    glEnd();
//    glFlush();
//}
//
//
//void drawLine(double x1, double y1, double x2, double y2, Color3<float> color) {
//
//    glLineWidth(1);//�����߶ο��
//    glBegin(GL_LINES);
//    glColor3f(color.r / 255.0, color.g / 255.0, color.b / 255.0);
//    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //�������귶Χ
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
//    glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //����ʹ������ͶӰ
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
//    //����һ����ת���ٶȳ�
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
//    glutKeyboardFunc(key);       //���̰���ȥʱ
//    glutIdleFunc(idle);          //����ʱ
//    glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
//    glutDisplayFunc(display);    //���ƴ�zz����ʾʱ
//
//    glutMainLoop();
//
//
//
//
//
// // //������д���ļ�
// ////�ǵ��������ʱ��Ҫɾ�� ԭ�����ļ���
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
// //    //Ȼ����д����������
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
