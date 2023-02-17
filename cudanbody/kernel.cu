//#include "helper_cuda.h"
#include "math_functions.h"
#include "math_constants.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <GL/glut.h>
#include <stdio.h>



const float SCREEN_SIZE = 400;
const float DRAW_SIZE = SCREEN_SIZE / 200 * 10;
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
	glPointSize(7.5f);//缺省是1
	glBegin(GL_POINTS);
	glColor3f(1, 128.0 / 255, 51.0 / 255);
	glVertex3f((x - 1) * DRAW_SIZE, (y - 1) * DRAW_SIZE, 0);
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


static void display(void)
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

	drawPoint(1, 1);

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




/*********************以下为CUDA专区*********************/

__device__ float getSofteningSquared() {
	return 0.001f;
}


__device__
float3 bodyPairInteraction(float4 pi, float4 pj, float3 ai)
{
	float3 distVector = { 0.0f, 0.0f, 0.0f };
	distVector.x = pj.x - pi.x;
	distVector.y = pj.y - pi.y;
	distVector.z = pj.z - pi.z;

	float distSquared = distVector.x * distVector.x + distVector.y * distVector.y
		+ distVector.z * distVector.z;
	distSquared += getSofteningSquared();
	float invDist = rsqrtf(distSquared);
	float invDistCubic = invDist * invDist * invDist;
	float coff = pj.w * invDistCubic;

	ai.x += distVector.x * coff;
	ai.y += distVector.y * coff;
	ai.z += distVector.z * coff;
	return ai;
}

__device__
float3 calcGravitationForce(float4 bodyPos, float4* positions, int numTiles, thread_block cta)
{
	extern __shared__ float4 sharedPos[];

	float3 acc = { 0.0f, 0.0f, 0.0f };

	for (unsigned int tile = 0; tile < numTiles; ++tile)
	{
		sharedPos[threadIdx.x] = positions[tile * blockDim.x + threadIdx.x];
		sync(cta);

		// tile calculation.
#pragma unroll 128
		for (unsigned int index = 0; index < blockDim.x; ++index)
		{
			acc = bodyPairInteraction(bodyPos, sharedPos[index], acc);
		}
		sync(cta);
	}

	return acc;
}


/*********************以上为CUDA专区*********************/
int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_SIZE, SCREEN_SIZE);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("n-body");

	glClearColor(6 / 255.0, 133 / 255.0, 135 / 255.0, 1);
	glShadeModel(GL_FLAT);



	return 0;

}
