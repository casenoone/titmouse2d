//#include <iostream>
//#include <fstream>
//#include <cmath>
//#include <string>
//#include <vector>
//#include <memory>
//using namespace std;
//
//#include "../../ConstVar.h"
//
//#include <GL/glut.h>
//#include <cmath>
//
//#include "../../Color3.hpp"
//#include "../../Vector2.hpp"
//
//#include "../../Geometry/Box2.h"
//
//#include "../../Lagrangian/SphSystemSolver2.h"
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
//
//
//
//
//auto sphSolver = make_shared<SphSystemSolver2>();
//
//int numberOfParticles = 0;
//
//
//static void display(void)
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glLoadIdentity();
//    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//    sphSolver->onAdvanceTimeStep(0.01);
//    for (int i = 0; i < numberOfParticles; ++i) {
//        auto pos = sphSolver->sphSystemData()->positions();
//        drawPoint(pos[i].x, pos[i].y);
//        
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
//    int res_x = 20;
//    int res_y = 20;
//
//    vector<Vector2<double>> temp_pos;
//
//    for (double i = 0.5; i < 1.5; i+=0.02) {
//        for (double j = 0.4; j < 1.6; j+=0.02) {
//            Vector2<double> temp(i, j);
//            temp_pos.push_back(temp);
//            numberOfParticles++;
//        }
//    }
//
//    ArrayPtr<Vector2<double>> pos(temp_pos);
//
//    sphSolver->setData(numberOfParticles, pos, res_x, res_y);
//
//    Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(2.0, 2.0), true);
//
//    Box2Ptr box2 = make_shared<Box2>(Vector2<double>(0.3, 0.3), Vector2<double>(0.6, 0.6), false);
//
//
//    Collider2 collider;
//
//    collider.push(box1);
//    //collider.push(box2);
//    sphSolver->setCollider(collider);
//
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
//    // //������д���ļ�
//    ////�ǵ��������ʱ��Ҫɾ�� ԭ�����ļ���
//    //int frame = 1000;
//    ////auto num = flipSolver->particleSystemData()->numberOfParticles();
//    ////auto position = flipSolver->particleSystemData()->positions();
//
//    //string outfilename = "1";
//
//    //int num = 0;
//
//    //system("mkdir LevelSet2");
//
//    //for (int i = 0; i < frame; i+= 1) {
//    //   
//    //    ofstream out("../titmouse2d/LevelSet2/"+outfilename + ".txt", ios::app);
//
//    //    auto sdfSize = levelsetLiquidSolver->sdf()->dataSize();
//    //    for (int i = 0; i < sdfSize.x; ++i) {
//    //        for (int j = 0; j < sdfSize.y; ++j) {
//    //            auto posFunc = levelsetLiquidSolver->sdf()->dataPosition();
//    //            auto pos = posFunc(i, j);
//    //            if (levelsetLiquidSolver->sdf()->lookAt(i, j) <= 0) {
//    //                ++num;
//    //                //drawPoint(pos, 3.2f, Color3<float>(255, 0, 0));
//    //            }
//    //        }
//    //    }
//
//
//    //    //Ȼ����д����������
//    //    for (int i = 0; i < sdfSize.x; ++i) {
//    //        for (int j = 0; j < sdfSize.y; ++j) {
//    //            auto posFunc = levelsetLiquidSolver->sdf()->dataPosition();
//    //            auto pos = posFunc(i, j);
//    //            if (levelsetLiquidSolver->sdf()->lookAt(i, j) <= 0) {
//    //                out << pos.x << "," << pos.y << endl;
//    //            }
//    //        }
//    //    }
//
//    //    levelsetLiquidSolver->onAdvanceTimeStep(0.02);
//    //    auto temp1 = std::atoi(outfilename.c_str());
//    //    temp1++;
//    //    outfilename = std::to_string(temp1);
//    //    
//    //}
//
//    //sphSystemData = std::dynamic_pointer_cast<SphSystemData2>(_particleSystemData);
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
