//#include <iostream>
//#include <fstream>
//#include <cmath>
//#include <string>
//
//#include "../../titmouse2d/titmouse2d/src/random.h"
//#include <GL/glut.h>
//#include <cmath>
//#include <cstdio>
//#include <cuda_runtime.h>
//#include "helper_cuda.h"
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
//	glPointSize(2.5f);//ȱʡ��1
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
//
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
///********************����ΪCUDA����********************/
////__global__ void kernel() {
////	unsigned int tid = blockDim.x * blockIdx.x + threadIdx.x;
////	unsigned int tnum = blockDim.x * gridDim.x;
////	printf("Thread %d %d\n", tid, tnum);
////
////}
//
//
////ͳһ�ڴ漼��
//__global__ void kernel(int* arr, int n) {
//	for (int i = 0; i < n; ++i) {
//		arr[i] = i;
//	}
//}
//
//
///********************����ΪCUDA����********************/
//
//
//
//
//
//
//static void display(void) {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);
//
//	/**********************����Ϊ��ͼ��**********************/
//
//	drawPoint(1, 1);
//	/**********************����Ϊ��ͼ��**********************/
//
//
//	glutSwapBuffers();
//
//}
//
//int main(int argc, char** argv)
//{
//
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("titmouse2d");
//	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
//	glShadeModel(GL_FLAT);
//
//
//
//
//	/**********************����Ϊ��������**********************/
//
//	int n = 32;
//	int* arr;
//	checkCudaErrors(cudaMallocManaged(&arr, n * sizeof(int)));
//
//	kernel << <1, 1 >> > (arr, n);
//	checkCudaErrors(cudaDeviceSynchronize());
//
//	for (int i = 0; i < n; ++i) {
//		printf("%d", arr[i]);
//	}
//	cudaFree(arr);
//
//	/**********************����Ϊ��������**********************/
//
//
//
//
//
//	glutKeyboardFunc(key);       //���̰���ȥʱ
//	glutIdleFunc(idle);          //����ʱ
//	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
//	glutDisplayFunc(display);    //���ƴ�����ʾʱ
//
//	glutMainLoop();
//
//
//
//
//
//
//	return 0;
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
