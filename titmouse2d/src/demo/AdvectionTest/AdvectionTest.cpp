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
#include "../../../src/Eulerian/CellCenteredVectorGrid2.hpp"


//advection����
//���Գ�����

//һ����ת��Բ�Σ�����һ���Ǻ�ɫ��һ���ǰ�ɫ
//���������Ժ���׷�١��е㷨
//�ٶȳ��Ĵ洢��ʹ��MAC����
//��ɫ��������������

struct Circles {
    Vector2<double> center;
    double r;
    vector<Color3<float>> colors;
    vector<Vector2<double>> points;
};


void rotation(Circles& circle,double delta) {
    
    delta *= (kPiD / 180);
    
    int num = circle.points.size();
    for (int i = 0; i < num; ++i) {
        auto x = circle.points[i].x - 1.0;
        auto y = circle.points[i].y - 1.0;
        
        circle.points[i].x = x * cos(delta) - y * sin(delta) + 1.0;
        circle.points[i].y = x * sin(delta) + y * cos(delta) + 1.0;

    }
}

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


void drawLine(double x1, double y1, double x2, double y2) {

    glLineWidth(1);//�����߶ο��
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //�������귶Χ
    glVertex2f((x2 - 1) * 10, (y2 - 1) * 10);
    glEnd();
    glFlush();
}



Circles circle;



static void display(void)
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

    rotation(circle, 0.1);

    int num = circle.points.size();
    for (int i = 0; i < num; ++i) {
       
        drawPoint(circle.points[i], 1.0f, circle.colors[i]);
    }

  
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

    Color3<float> black(0, 0, 0);
    Color3<float> white(255, 255, 255);
    

    double r = 0.7;
    double r2 = r * r;
    Vector2<double> center(1.0, 1.0);

    circle.center = center;
    circle.r = r;

    double interval = 0.01;

    for (double i = 0; i < 2; i += interval) {
        for (double j = 0; j < 2; j += interval) {
            Vector2<double> p(i, j);
            if (p.disSquare(center) <= r) {
                circle.points.push_back(p);
            }
        }
    }

    int num = circle.points.size();
    for (int i = 0; i < num; ++i) {
        
        if (circle.points[i].x - 1.0 <= 0) {
            circle.colors.push_back(black);
        }
        else {
            circle.colors.push_back(white);
        }
    }





    glutKeyboardFunc(key);       //���̰���ȥʱ
    glutIdleFunc(idle);          //����ʱ
    glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
    glutDisplayFunc(display);    //���ƴ�����ʾʱ

    glutMainLoop();



    return 0;
}


















