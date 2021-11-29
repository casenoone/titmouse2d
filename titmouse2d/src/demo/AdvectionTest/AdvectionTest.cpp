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


//advection测试
//测试场景：

//一个旋转的圆形，其中一半是黑色，一半是白色
//方法：线性后向追踪、中点法
//速度场的存储：使用MAC网格
//颜色场存于中心网格

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
    case 27: //按ESC
    case 'q': //按q键程序都会退出
        exit(0);
        break;
    }

    glutPostRedisplay();  //产生重绘回调
}

void drawPoint(double x, double y)
{
    //在后缓存绘制图形，就一个点
    glPointSize(2.05f);//缺省是1
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

    glLineWidth(1);//设置线段宽度
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f((x1 - 1) * 10, (y1 - 1) * 10); //定点坐标范围
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
    glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //这里使用正视投影
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





    glutKeyboardFunc(key);       //键盘按下去时
    glutIdleFunc(idle);          //空闲时
    glutReshapeFunc(resize);     //改变窗口大小时
    glutDisplayFunc(display);    //绘制窗口显示时

    glutMainLoop();



    return 0;
}


















