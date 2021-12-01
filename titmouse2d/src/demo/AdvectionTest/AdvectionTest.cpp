#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;

#include "../../ConstVar.h"

#include <GL/glut.h>
#include <cmath>

#include "../../Color3.hpp"
#include "../../Vector2.hpp"
#include "../../Eulerian/CellCenteredVectorGrid2.hpp"
#include "../../Eulerian/FaceCenteredGrid2.h"
#include "../../Eulerian/AdvectionSolver2.h"

//advection����
//���Գ�����

//һ����ת��Բ�Σ�����һ���Ǻ�ɫ��һ���ǰ�ɫ
//���������Ժ���׷�١��е㷨
//�ٶȳ��Ĵ洢��ʹ��MAC����
//��ɫ��������������



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
    glPointSize(2.05f);//ȱʡ��1
    glBegin(GL_POINTS);
    glColor3f(1, 128.0 / 255, 51.0 / 255);
    glVertex3f((x - 1) * 10, (y - 1) * 10, 0);
    glEnd();
}

void drawPoint(Vector2<double> pos,float size, Color3<float> color)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor3f(color.r/255.0, color.g / 255.0, color.b / 255.0);
    glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
    glEnd();
}

void drawPoint(Vector2<double> pos, float size, Color3<double> color)
{
    glPointSize(size);
    glBegin(GL_POINTS);
    glColor3f(float(color.r) / 255.0, float(color.g) / 255.0, float(color.b) / 255.0);
    glVertex3f((pos.x - 1) * 10, (pos.y - 1) * 10, 0);
    glEnd();
}



void drawLine(double x1, double y1, double x2, double y2) {

    glLineWidth(1);//�����߶ο��
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //�������귶Χ
    glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
    glEnd();
    glFlush();
}


Vector2<size_t> resolution(50, 50);
Vector2<double> origin(0.0, 0.0);
Color3<double> red(255, 0.0, 0.0);

CellCenteredVectorGrid2Ptr<Color3<double>> advectedData;


Vector2<double> center1(1.0, 1.0);
double r1 = 0.5;
double r1_sqr = r1 * r1;

Color3<double> blacks(0, 0, 0);
Color3<double> whites(255, 255, 255);


auto velocity = make_shared<FaceCenteredGrid2>(resolution, origin, Vector2<double>(0, 0));

auto advectionSolver = make_shared<AdvectionSolver2>();

static void display(void)
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


    for (int i = 0; i < advectedData->resolution().x; ++i) {
        for (int j = 0; j < advectedData->resolution().y; ++j) {
            auto dataFunc = advectedData->dataPosition();
            auto pos = dataFunc(i, j);
            auto color = (*advectedData)(i, j);
            drawPoint(pos, 4.0f, color);
        }
    }

    advectionSolver->solve(velocity, advectedData, 0.1);
  
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
    glutInitWindowSize(200, 200);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("titmouse2d");

    glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);   
    glShadeModel(GL_FLAT);


   
   

    advectedData = make_shared<CellCenteredVectorGrid2<Color3<double>>>(resolution, origin, red);

    for (int i = 0; i < advectedData->resolution().x; ++i) {
        for (int j = 0; j < advectedData->resolution().y; ++j) {
            auto posFunc = advectedData->dataPosition();
            auto current_pos = posFunc(i, j);
            if (current_pos.disSquare(center1) <= r1_sqr) {
                if (current_pos.x - center1.x <= 0.0) (*advectedData)(i, j) = blacks;
                else (*advectedData)(i, j) = whites;

            }
        }
    }



    //����һ����ת���ٶȳ�
    auto velSizeU = velocity->uSize();

    for (int i = 0; i < velSizeU.x; ++i) {
        for (int j = 0; j < velSizeU.y; ++j) {
            auto posFunc = velocity->uPosition();
            auto pos = posFunc(i, j);
            auto tempVec = (pos - center1).getNormalize();
            velocity->u(i,j) = tempVec.x * cos(kPiD / 2) - tempVec.y * sin(kPiD / 2);
        }
    }

    auto velSizeV = velocity->vSize();

    for (int i = 0; i < velSizeV.x; ++i) {
        for (int j = 0; j < velSizeV.y; ++j) {
            auto posFunc = velocity->vPosition();
            auto pos = posFunc(i, j);
            auto tempVec = (pos - center1).getNormalize();
            velocity->v(i, j) = tempVec.x * sin(kPiD / 2) + tempVec.y * cos(kPiD / 2);
        }
    }


   
    glutKeyboardFunc(key);       //���̰���ȥʱ
    glutIdleFunc(idle);          //����ʱ
    glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
    glutDisplayFunc(display);    //���ƴ�zz����ʾʱ

    glutMainLoop();



    return 0;
}


















