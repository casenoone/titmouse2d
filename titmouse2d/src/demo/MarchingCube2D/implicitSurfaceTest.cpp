//#include <iostream>
//using namespace std;
//#include "../../Array.hpp"
//#include "../../Eulerian/MarchingCubes2.h"
//
//#include "../../Eulerian/VertexCenteredScalarGrid2.h"
//
//#include "../../Geometry/Sphere2.h"
//#include <GL/glut.h>
//
//#include "../../Color3.hpp"
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
//    glColor3f(color.r/255.0, color.g / 255.0, color.b / 255.0);
//    glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
//    glEnd();
//}
//
//void drawLine(double x1, double y1, double x2, double y2) {
//
//    glLineWidth(2);//�����߶ο��
//    glBegin(GL_LINES);
//    glColor3f(1.0, 0.0, 0.0);
//    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //�������귶Χ
//    glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
//    glEnd();
//    glFlush();
//}
//
//Vector2<size_t> resolution(30, 30);
//Vector2<double> origin(0.0, 0.0);
//
//auto mc = MarchingCube2::builder()
//.withResolution(resolution)
//.withInitialValue(1.0)
//.withOrigin(0.0, 0.0)
//.makeShared();
//
//Vector2<double> center1(1.0, 1.0);
//double r1 = 0.2;
//
//auto sphere1 = Sphere2(center1, r1, resolution, origin, 0.0);
//
//
//double t = 0.0;
//
//static void display(void)
//{
//
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glLoadIdentity();
//    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//    t += 0.05;
//
//    vector<LineSegment> lineSet;
//    mc->getLineSegmentSet(lineSet, sphere1.sdf());
//  
//
//    for (auto i = lineSet.begin(); i != lineSet.end(); ++i) {
//        auto start = i->start;
//        auto end = i->End;
//
//        drawLine(start.x, start.y, end.x, end.y);
//    }
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
//    glutCreateWindow("titmouse");
//
//    glClearColor(0.0, 0.0, 0.0, 1);
//    glShadeModel(GL_FLAT);
//
//
//    
//
//    //����Բ�ķ��ž��볡�����ڶ���������
//    
//
//
//
//
//
//    glutKeyboardFunc(key);       //���̰���ȥʱ
//    glutIdleFunc(idle);          //����ʱ
//    glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
//    glutDisplayFunc(display);    //���ƴ�����ʾʱ
//
//    glutMainLoop();
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
