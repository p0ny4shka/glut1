#define GLUT_DISABLE_ATEXIT_HACK
#include "freeglut.h"
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstring>
#include <windows.h>
#include "Header.h"


void draw(GLfloat red, GLfloat green, GLfloat blue)
{
    glClear(GL_COLOR_BUFFER_BIT);//clear buffer
    glMatrixMode(GL_MODELVIEW);
    glLineWidth(1.3);
    for (int i = 0; i < p; i++) {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINE_LOOP);
        for (int s = 0; s < 4; s++)
            glVertex3f(models[i].point[s].x, models[i].point[s].y, models[i].point[s].z);
        glEnd();

        glColor3f(red, green, blue);
        glBegin(GL_QUADS);
        for (int s = 0; s < 4; s++)
            glVertex3f(models[i].point[s].x, models[i].point[s].y, models[i].point[s].z);
        glEnd();
    }
}

void reshape(int w, int h)
{

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);

}

void display() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.8, 0.8, 0.8, 0.0);  //background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(tx, ty, tz);		//model movement
    glRotatef(rx, 1, 0, 0); //model rotation
    glRotatef(ry, 0, 1, 0);
    glScalef(size, size, size);		//model size change
    draw(red, green, blue);
    glPopMatrix();
    glutSwapBuffers();
}


void update()
{

}

void runMainLoop(int val)
{
    update();
    display();
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
}





void compare(float* coords, int* indices, int countFase, int countPoint)
{
    dot = new coord[countPoint / 3];
    models = new polygons[countFase/4];

    for (int i = 0; i < countPoint; i += 3)
    {
        dot[point].x = coords[i + 0]; //coord Õ
        dot[point].y = coords[i + 1]; //coord Y
        dot[point].z = coords[i + 2]; //coord Z
        point++;
    }
    p = 0;
    for (int f = 0; f < countFase; f += 4)
    {
        models[p].point = new coord[4];
        for (int s = 0; s < 4; s++)
            models[p].point[s] = dot[indices[f + s]];
        p++;
    }
}

void readFile(const char* fileName)
{
    file.open(fileName);

    if (file.is_open())
    {
        std::cout << "file opened";
        file.seekg(0, std::ios::end); //sets cursof at the end of the file
        int fileSize = file.tellg(); //sets size of temp array for data
        float* tempCoords = (float*)malloc(fileSize); //temp array for coords
        int* tempFaseArray = (int*)malloc(fileSize); //temp array for dots, needed to create surfaces
        coordsIndex = 0;
        faseIndex = 0;
        int countPoint = 0; //dots counter
        int countFase = 0; // surfaces counter

        file.seekg(0, std::ios::beg);
        while (!file.eof()) {
            std::getline(file, line);
            if ((line.c_str()[0] == 'v') && (line.c_str()[1] == ' '))
            {
                line[0] = ' ';
                line[1] = ' ';
                sscanf_s(line.c_str(), "%f %f %f ",
                    &tempCoords[coordsIndex + 0],   // X coord
                    &tempCoords[coordsIndex + 1], // Y coord
                    &tempCoords[coordsIndex + 2]  // Z coord
                );
                coordsIndex += 3;
                countPoint++;
            }
            if ((line.c_str()[0] == 'f') && (line.c_str()[1] == ' '))
            {
                line[0] = ' ';
                int tempPoint[4], tempNormal[4], tempTexture[4];
                sscanf_s(line.c_str(), "%i/%i/%i%i/%i/%i%i/%i/%i%i/%i/%i",
                    // dot number||normal number||texture number
                    &tempPoint[0], &tempNormal[0], &tempTexture[0]
                    , &tempPoint[1], &tempNormal[1], &tempTexture[1]
                    , &tempPoint[2], &tempNormal[2], &tempTexture[2]
                    , &tempPoint[3], &tempNormal[3], &tempTexture[3]
                );

                tempFaseArray[faseIndex + 0] = tempPoint[0]; //first dot index
                tempFaseArray[faseIndex + 1] = tempPoint[1]; //second dot index
                tempFaseArray[faseIndex + 2] = tempPoint[2]; //third dot index
                tempFaseArray[faseIndex + 3] = tempPoint[3]; //fourth dot index

                faseIndex += 4;
                countFase++;
            }
        }
        compare(tempCoords, tempFaseArray, faseIndex, coordsIndex);
        free(tempCoords);
        free(tempFaseArray);
    }
    else
    {
        std::cout << "file isn't opened";
    }

}

void mouseMotion(int x, int y)
{
    if (ldown)		// LMB
    {
        rx += 0.5 * (y - my);	//angle change
        ry += 0.5 * (x - mx);
        mx = x;
        my = y;
       	glutPostRedisplay();	//redraw screen
    }


    if (rdown)	//RMB
    {
        tx += 0.01 * (x - mx);	
        if (tt)
            tz += 0.01 * (y - my);
        else
            ty += 0.01 * (my - y);
        mx = x;
        my = y;
        if (tx > 0.8)
            tx = 0.8;
        if (tx < -0.8)
            tx = -0.8;
        if (ty > 0.8)
            ty = 0.8;
        if (ty < -0.8)
            ty = -0.8;
    }
}

void mouseWheel(int wheel, int direction, int x, int y)
{
    if (direction == -1)
        size += 0.2;
    if (direction == 1)
        size -= 0.2;
    if (size < 0.3)
        size = 0.4;
}

void mouse(int button, int state, int x, int y)		//Mouse button clicks
{

    if (button == GLUT_LEFT_BUTTON)		//LMB
    {
        switch (state)
        {
        case GLUT_DOWN:		
            ldown = true;		
            mx = x;			
            my = y;
            break;
        case GLUT_UP:
            ldown = false;
            break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)	//RMB
    {
        switch (state)
        {
        case GLUT_DOWN:
            rdown = true;
            mx = x;
            my = y;
            break;
        case GLUT_UP:
            rdown = false;
            break;
        }
    }
}

void InitOpenGL(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Window");
   // glutFullScreen();
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(Keyboard);
    glutReshapeFunc(reshape);
    glMatrixMode(GL_PROJECTION);

   // glLoadIdentity();

    glClear(GL_COLOR_MATERIAL);//clears buffer
    glutDisplayFunc(display);//render

    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);
    glutMainLoop();
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {

    case 'z':
        rx = 90;
        ry = 0;
        break;
    case 'x':
        rx = 0;
        ry = 90;
        break;
    case 'c':
        rx = 0;
        ry = 0;
        break;
    case '1':
        red = 0.6;
        green = 0;
        blue = 0;
        break;
    case '2':
        red = 0;
        green = 0.5;
        blue = 0;
        break;
    case '3':
        red = 1;
        green = 1;
        blue = 1;
        break;
    case '4':
        red = 0;
        green = 0.5;
        blue = 0.8;
        break;
    case '5':
        red = 0.8;
        green = 0.1;
        blue = 0.4;
        break;
    case VK_ESCAPE:
        if (isFullScr == true)
        {
            glutReshapeWindow(1280, 720);
            isFullScr = false;
        }
        else
            exit(0);
        break;
    case 'f':
        if (isFullScr == true)
        {
            glutReshapeWindow(1280, 720);
            isFullScr = false;
        }
        glutFullScreen();
        isFullScr = true;
        break;
    }
}

void instruction()
{
    std::cout << std::endl;
    std::cout << "To change color, use keyboard numbers : " << std::endl << "1. red" << std::endl << "2. green" << std::endl << "3. white" << std::endl << "4. blue" << std::endl << "5. pink" << std::endl;
    std::cout << "Press 'F' to fullscreen mode, press 'ESC' or 'F' again to window mode" << std::endl;
    std::cout << "Press 'Z' to change projection to Z axis" << std::endl;
    std::cout << "Press 'X' to change projection to X axis" << std::endl;
    std::cout << "Press 'C' to change projection to Y axis" << std::endl;
    std::cout << "Hold 'LMB' to rotate model" << std::endl;
    std::cout << "Hold 'RMB' to move model" << std::endl;
    std::cout << "Scroll to change model size" << std::endl;
}


int main(int argc, char* argv[])
{
    readFile("cube2.obj");
    instruction();
    InitOpenGL(argc, argv);
    return 0;
}