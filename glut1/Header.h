#pragma once
const int SCREEN_FPS = 60;

std::string line;
std::fstream file;
float* coords;
int* index;
int  coordsIndex = 0, faseIndex = 0; //indices of dots in .obj

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

coord* dot; //new structure that stores dot coords
polygons* models; //new structure thst stores dots coords, to draw lines
int point = 1; //dots counter
int p = 0; //coords indeces



//glut vars
GLfloat  tx = 0;			// move x
GLfloat	 ty = 0;			// y
GLfloat	 tz = 0;			// z 
GLfloat  rx = 0;			// rotation angle x
GLfloat  ry = 0;			// y
GLint	 tt = 0;			// active dimention: 0 - XY, 1 - XZ
GLfloat red = 0.5;
GLfloat green = 0.5;
GLfloat blue = 0.5;

int mx = 0, my = 0;				// mouse coords x;y
bool ldown = false,		
rdown = false;
float size = 1;
bool mwheeldown = false,
mwheelup = false;
bool isFullScr = false;



//functions

void InitOpenGL(int argc, char* argv[]);

void Keyboard(unsigned char key, int x, int y);

void draw(GLfloat red, GLfloat green, GLfloat blue);

void display();

void mouse(int button, int state, int x, int y);

void mouseWheel(int wheel, int direction, int x, int y);

void readFile(const char* fileName);

void compare(float* coords, int* indexes, int countFase, int countPoint);

void runMainLoop(int); //when frame gets updated, update it 

void instruction();

void reshape(int, int); //resize window with saving perspective
