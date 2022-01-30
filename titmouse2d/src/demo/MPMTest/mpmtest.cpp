#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;


#include "../../Lagrangian/ParticleSystemSolver2.h"
#include "../../Geometry/Box2.h"
#include "../../Geometry/Plane2.h"

#include "../../OtherMethod/MPM/MPMSolver2.h"
#include "../../Matrix2x2.hpp"
#include <GL/glut.h>
#include <cmath>

#include "../../random.h"

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
	glPointSize(3.0f);//ȱʡ��1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glVertex3f((x - 1) * 10, (y - 1) * 10, 0);
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



Vector2<size_t> mpm_res(90, 90);
Vector2D mpm_gs(2.0 / mpm_res.x, 2.0 / mpm_res.x);
Vector2D mpm_origin(0.0, 0.0);

auto mpmSolver = make_shared<MPMSolver2>(mpm_res, mpm_gs, mpm_origin);

double dt = 2e-4;


static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);


	auto num = mpmSolver->mpmData()->numberOfParticles();
	auto& pos = mpmSolver->mpmData()->positions();
	for (size_t i = 0; i < num; ++i) {
		drawPoint(pos[i].x, pos[i].y);
	}

	mpmSolver->onAdvanceTimeStep(dt);
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


	int numberOfParticles = 7000;
	int resolutionX = 10;
	int resolutionY = 10;
	vector <Vector2<double>> temp1;
	for (int i = 0; i < numberOfParticles; ++i) {
		auto x = random_double(0.7, 1.3);
		auto y = random_double(0.1, 1.9);
		Vector2<double> temp(x, y);
		temp1.push_back(temp);
	}

	ArrayPtr<Vector2<double>> pos(temp1);


	Box2Ptr box1 = make_shared<Box2>(Vector2<double>(0, 0), Vector2<double>(2.0, 2.0), true);
	Box2Ptr box2 = make_shared<Box2>(Vector2<double>(0.1, 0.1), Vector2<double>(1.9, 1.9), true);
	Box2Ptr box3 = make_shared<Box2>(Vector2<double>(0.6, 0.6), Vector2<double>(1.0, 0.7), false);
	Plane2Ptr plane1 = make_shared<Plane2>(Vector2<double>(0.7, 0.8), Vector2<double>(1.0, 0.8), false);



	mpmSolver->setData(numberOfParticles, pos, resolutionX, resolutionY);



	//������д���ļ�
//�ǵ��������ʱ��Ҫɾ�� ԭ�����ļ���
	int frame = 9999999999999999;
	auto num = mpmSolver->mpmData()->numberOfParticles();
	auto position = mpmSolver->mpmData()->positions();


	int interval = 1;

	string outfilename = "1";

	system("mkdir MPMData");

	for (int i = 0; i < frame; i += 1) {

		ofstream out("../titmouse2d/MPMData/" + outfilename + ".txt", ios::app);

		for (int n = 0; n < num; ++n) {
			auto x = position[n].x;
			auto y = position[n].y;
			out << x << "," << y << endl;
		}
		mpmSolver->onAdvanceTimeStep(dt);
		auto temp1 = std::atoi(outfilename.c_str());
		temp1++;
		outfilename = std::to_string(temp1);

	}














	glutKeyboardFunc(key);       //���̰���ȥʱ
	glutIdleFunc(idle);          //����ʱ
	glutReshapeFunc(resize);     //�ı䴰�ڴ�Сʱ
	glutDisplayFunc(display);    //���ƴ�����ʾʱ

	//glutMainLoop();



	return 0;
}


















