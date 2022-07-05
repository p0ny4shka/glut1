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


void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);//clear buffer
    glMatrixMode(GL_MODELVIEW);
    glLineWidth(1);
    for (int i = 0; i < p; i++) {
        glColor3f(0, 0.5, 0);
        glBegin(GL_LINE_LOOP);
        for (int s = 0; s < 4; s++)
            glVertex3f(models[i].point[s].x, models[i].point[s].y, models[i].point[s].z);
        glEnd();

        glColor3f(0, 1, 0);
        glBegin(GL_TRIANGLES);
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
    glColor3f(0.1, 0.7, 0.2);
    glClearColor(0.5, 0.5, 0.75, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(tx, ty, tz);		//Перемещение и поворт объекта
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glScalef(size, size, size);		//Перемещение и поворт объекта
    draw();			//Вывод объекта на экран
    glPopMatrix();
    glutSwapBuffers();
}


void update()
{

}

void runMainLoop(int val)
{
    //Frame logic
    update();
    display();

    //Run frame one more time
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, val);
}





void compare(float* coords, int* indices, int countFase, int countPoint)
{
    dot = new coord[countPoint / 3];
    models = new polygons[countFase/4];

    for (int i = 0; i < countPoint; i += 3)
    {
        dot[point].x = coords[i + 0]; //координата Х
        dot[point].y = coords[i + 1]; //координата Y
        dot[point].z = coords[i + 2]; //координата Z
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

void MouseMotion(int x, int y)	//Перемешение мыши
{
    if (ldown)		// Левая кнопка
    {
        rx += 0.5 * (y - my);	//Изменение угола поворота
        ry += 0.5 * (x - mx);
        mx = x;
        my = y;
        //	glutPostRedisplay();	//Перерисовать экран
    }
}

void Mouse(int button, int state, int x, int y)		//Обработка щелчков мыши
{

    if (button == GLUT_LEFT_BUTTON)		//Левая кнопка
    {
        switch (state)
        {
        case GLUT_DOWN:		//Если нажата
            ldown = true;		//установить флаг
            mx = x;			//Запомнить координаты
            my = y;
            break;
        case GLUT_UP:
            ldown = false;
            break;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)	//Правая кнопка
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
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Window");
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseMotion);

    glutReshapeFunc(reshape);
    glMatrixMode(GL_PROJECTION);

   // glLoadIdentity();

    glClear(GL_COLOR_MATERIAL);//clears buffer
    glutDisplayFunc(display);//render
    glutTimerFunc(1000 / SCREEN_FPS, runMainLoop, 0);
    glutMainLoop();
}



int main(int argc, char* argv[])
{
    readFile("cube2.obj");

    InitOpenGL(argc, argv);
    return 0;
}