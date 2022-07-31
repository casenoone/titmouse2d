#include <iostream>

#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <vector>

#include "../titmouse2d/src/Geometry/RegularPolygon.h"
#include "../titmouse2d/src/Color3.hpp"
#include <GL/glut.h>

#include <windows.h>
#include <omp.h>


//stb所需数据
const int width = 600;
const int height = 600;
#define CHANNEL_NUM 4

const double kPiD = 3.1415926535;

RegularPolygonPtr obj1 = std::make_shared<RegularPolygon>(21, Vector2D(0.1, 1), 0.06);





const float SCREEN_SIZE = 600;
const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;
void split(const std::string& s, std::vector<std::string>& tokens, char delim = ' ') {
	tokens.clear();

	auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
#pragma omp parallel num_threads(23)
		for (size_t i = pos; i < s.size(); i++) {
			if (s[i] != delim) return i;
		}
		return std::string::npos;
	};
	size_t lastPos = string_find_first_not(0);
	size_t pos = s.find(delim, lastPos);
	while (lastPos != std::string::npos) {
		tokens.emplace_back(s.substr(lastPos, pos - lastPos));
		lastPos = string_find_first_not(pos);
		pos = s.find(delim, lastPos);
	}
}


std::string filename = "1";
int PAUSE = 1;

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //按ESC
	case 'q': //按q键程序都会退出
		exit(0);
		break;
	case 'r':
		filename = "1";
		break;

	}


	glutPostRedisplay();  //产生重绘回调
}

void drawPoint(double x, double y)
{
	//在后缓存绘制图形，就一个点
	glPointSize(1.5f);//缺省是1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glColor3f(1, 1, 1);
	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
	glEnd();
}

void drawCircle(const Vector2D& center, double r, int res) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(1, 128.0 / 255, 51.0 / 255);
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(1, 1, 1);
		glVertex2f(x, y);
	}
	glEnd();
}


void drawCircle(const Vector2D& center, double r, int res, Color3<double> color) {
	glBegin(GL_POLYGON);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(color.r / 255, color.g / 255, color.b / 255);
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < res; ++i) {
		auto x = (r * cos(2 * kPiD / res * i)) + center.x;
		auto y = (r * sin(2 * kPiD / res * i)) + center.y;
		x = (x - 1) * DRAW_SIZE;
		y = (y - 1) * DRAW_SIZE;

		glColor3f(1, 1, 1);
		glVertex2f(x, y);
	}
	glEnd();
}




void drawLine(double x1, double y1, double x2, double y2) {

	glLineWidth(1);//设置线段宽度
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f((x1 - 1) * DRAW_SIZE, (y1 - 1) * DRAW_SIZE); //定点坐标范围
	glVertex2f((x2 - 1) * DRAW_SIZE, (y2 - 1) * DRAW_SIZE);
	glEnd();
	glFlush();
}



bool clearState = true;
double dt = 0.006;



static void display(void)
{



	//动画实现是用双缓存，现在用背景色擦除后缓存
	if (clearState) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}



	//设置MODELVIEW矩阵，先设为单位阵，再乘上观察矩阵，即从z轴正向100处向 （0，0，0）
	//看，上方向(0,1,0)
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	//在这里读取粒子数据
	std::ifstream myfile("E:\\zhangjian\\solve_data\\0022\\" + filename + ".txt");

	if (myfile.is_open() == false) {
		system("pause");

		//myfile("E:\\zhangjian\\paper_and_project\\titmouse2d\\OpenGL\\FoamTest3\\" + filename + ".txt");
	}

	auto temp1 = std::atoi(filename.c_str());

	int skipNum = 2;
	temp1 += skipNum;
	filename = std::to_string(temp1);

	std::string strLine;
	std::vector<std::string> position;

	std::vector<Vector2D> posList;

	while (getline(myfile, strLine)) {
		if (strLine.empty()) {
			continue;
		}

		split(strLine, position, ',');
		auto x = atof(position[0].c_str());
		auto y = atof(position[1].c_str());
		Vector2D tempPos(x, y);
		if (tempPos.dis(obj1->center()) > obj1->r()) {
			//在这里写入像素
			//drawPoint(x, y);
			//drawCircle(Vector2D(x, y), 0.03, 20); 
			drawCircle(tempPos, 0.01, 20);
			//drawPoint(x, y);
		}
	}

	myfile.close();

	/*for (auto i = obj1->_data.begin(); i != obj1->_data.end(); ++i) {
		auto start = i->start;
		auto end = i->end;
		drawLine(start.x, start.y, end.x, end.y);
	}*/

	obj1->velocity = Vector2D(3, 0.0);
	obj1->updatePosition(dt * skipNum);

	//drawCircle(obj1->center(), obj1->r(), 50);
	drawCircle(obj1->center(), obj1->r(), 50, Color3<double>(0, 191, 255));

	//然后前后缓存交换 
	glutSwapBuffers();

	//延时0.5秒


	//Sleep(25);

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
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("titmouse2d");

	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_FLAT);



	glutKeyboardFunc(key);       //键盘按下去时
	glutIdleFunc(idle);          //空闲时
	glutReshapeFunc(resize);     //改变窗口大小时
	glutDisplayFunc(display);    //绘制窗zz口显示时

	glutMainLoop();



	return 0;
}


