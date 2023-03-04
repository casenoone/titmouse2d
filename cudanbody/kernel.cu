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
//	case 27: //��ESC
//	case 'q': //��q�����򶼻��˳�
//		exit(0);
//		break;
//	}
//
//	glutPostRedisplay();  //�����ػ�ص�
//}
//
//void drawPoint(double x, double y)
//{
//	//�ں󻺴����ͼ�Σ���һ����
//	glPointSize(7.5f);//ȱʡ��1
//	glBegin(GL_POINTS);
//	glColor3f(1, 128.0 / 255, 51.0 / 255);
//	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
//	glEnd();
//}
//
//void drawLine(double x1, double y1, double x2, double y2) {
//
//	glLineWidth(1);//�����߶ο��
//	glBegin(GL_LINES);
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //�������귶Χ
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
//	glOrtho(-width * 0.5 * ratio, width * 0.5 * ratio, -height * 0.5 * ratio, height * 0.5 * ratio, 2.0, 100.0); //����ʹ������ͶӰ
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//
//
//
//
///*********************����ΪCUDAר��*********************/
//
//__global__ void kernel() {
//
//}
//
//
///*********************����ΪCUDAר��*********************/
//int main(int argc, char** argv)
//{
//
//	/*********************����ΪOpenGL����*********************/
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("n-body");
//
//	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
//	glShadeModel(GL_FLAT);
//	/*********************����ΪOpenGL����*********************/
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

// ���ڴ�С
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// �ַ���
const char* text = "�����Ѷ����漸�Ѷ�";

// ��Ⱦ�ַ���
void renderText() {
	// �ַ�������
	int len = strlen(text);

	// ��������
	void* font = GLUT_BITMAP_HELVETICA_18;

	// ���ַ�����ȾΪλͼ
	glRasterPos2f(-0.8, 0.0);
	for (int i = 0; i < len; i++) {
		if (text[i] == '��') {
			// ���浱ǰ����
			glPushMatrix();

			// �á���������������ת
			glTranslatef(0.0, -0.02, 0.0);
			glRotatef(45.0 * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0), 0.0, 0.0, 1.0);
			glTranslatef(0.0, 0.02, 0.0);

			// ��Ⱦ��������
			glutBitmapCharacter(font, text[i]);

			// �ָ�֮ǰ�ľ���
			glPopMatrix();
		}
		else {
			// ��Ⱦ�����ַ�
			glutBitmapCharacter(font, text[i]);
		}
	}
}

// ��ʼ������
void onInit() {
	// ���ô��ڱ���ɫΪ��ɫ
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

// ���ƺ���
void onDraw() {
	// �����ɫ������
	glClear(GL_COLOR_BUFFER_BIT);

	// �����ַ���
	renderText();

	// ����ǰ�󻺳���
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	// ��ʼ��OpenGL��GLUT��
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("OpenGL Window");
	// ע���ʼ�������ͻ��ƺ���
	glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
	onInit();
	// ������ѭ��
	glutMainLoop();
	return 0;
}
