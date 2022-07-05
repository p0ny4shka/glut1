#pragma once
const int SCREEN_FPS = 60;
GLdouble x_angle = 1;
GLdouble y_angle = 1;
GLdouble z_angle = 1;

std::string line;
std::fstream file;
float* coords;
int* index;
int  coordsIndex = 0, faseIndex = 0; //indices of dots in .obj


//glut vars
GLfloat  tx = 0;			// ����� �� ��� X
GLfloat	 ty = 0;			// Y
GLfloat	 tz = 0;			// Z
GLfloat  rx = 0;			// ���� ������� ����� ������ ��� X
GLfloat  ry = 0;			// Y
GLint	 tt = 0;			// �������� ��������: 0 - XY, 1 - XZ
int mx = 0, my = 0;				// mouse coorde x;y
bool ldown = false,		// ������ ����� ������� ����?
rdown = false;
float size = 1;


struct coord
{
	float x;
	float y;
	float z;
};

struct polygons
{
	coord* point;
};
//new structure that stores dot coords
coord* dot;
//new structure thst stores dots coords, to draw lines
polygons* models;
int point = 1; //���������� �����
int p = 0; //�������� ���������� � �������

void draw();

void display();

void readFile(const char* fileName);

void compare(float* coords, int* indexes, int countFase, int countPoint);

void runMainLoop(int); //when frame gets updated, update it 


void reshape(int, int);
//resize window with saving perspective