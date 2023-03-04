////#include "helper_cuda.h"
//#include "math_functions.h"
//#include "math_constants.h"
//#include "cuda_runtime.h"
//#include "device_launch_parameters.h"
//#include <GL/glut.h>
//#include <stdio.h>
//
//
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
//	glPointSize(7.5f);//缺省是1
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
//static void display(void)
//{
//
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//	drawPoint(1, 1);
//
//	glutSwapBuffers();
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
//
///*********************以下为CUDA专区*********************/
//
//__global__ void kernel() {
//
//}
//
//
///*********************以上为CUDA专区*********************/
//int main(int argc, char** argv)
//{
//
//	/*********************以下为OpenGL配置*********************/
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("n-body");
//
//	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
//	glShadeModel(GL_FLAT);
//	/*********************以上为OpenGL配置*********************/
//
//
//
//
//
//
//
//
//
//	return 0;
//
//}

#include <GL/glut.h>
#include <math.h>
#include <string.h>

// 窗口大小
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 字符串
const char* text = "啊几把东，真几把东";

// 渲染字符串
void renderText() {
	// 字符串长度
	int len = strlen(text);

	// 设置字体
	void* font = GLUT_BITMAP_HELVETICA_18;

	// 将字符串渲染为位图
	glRasterPos2f(-0.8, 0.0);
	for (int i = 0; i < len; i++) {
		if (text[i] == '东') {
			// 保存当前矩阵
			glPushMatrix();

			// 让“东”字绕自身旋转
			glTranslatef(0.0, -0.02, 0.0);
			glRotatef(45.0 * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0), 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.02, 0.0);

			// 渲染“东”字
			glutBitmapCharacter(font, text[i]);

			// 恢复之前的矩阵
			glPopMatrix();
		}
		else {
			// 渲染其他字符
			glutBitmapCharacter(font, text[i]);
		}
	}
}

// 初始化函数
void onInit() {
	// 设置窗口背景色为白色
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// 绘制函数
void onDraw() {
	// 清除颜色缓冲区
	glClear(GL_COLOR_BUFFER_BIT);

	// 绘制字符串
	renderText();

	// 交换前后缓冲区
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	// 初始化OpenGL和GLUT库
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL Window");
	// 注册初始化函数和绘制函数
	glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
	onInit();
	// 进入主循环
	glutMainLoop();
	return 0;
}
