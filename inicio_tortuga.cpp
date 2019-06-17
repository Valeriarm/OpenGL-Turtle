/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * Integrantes:
 * Valeria Rivera Muñoz, 1626837
 * Juan Felipe Gil Londoño, 1626055
 * Mateo Gregory Jimenez, 1629431
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>


using namespace std;
bool flagAxis = false;
bool flagTurtle = true;
GLfloat X=0.0, Y=1.0 ,Z=3.0;

void drawTurtle(void){
    double x[]={0.0,0.1,0.2,0.2,0.1,0.1,0.2,0.2,0.3 ,0.4,0.4,0.3,0.4,0.4,0.3,0.4,0.4,0.3 ,0.2,0.1,0.0 },
           z[]={0.0,0.0,0.1,0.2,0.3,0.4,0.4,0.3,0.25,0.3,0.4,0.5,0.7,0.9,1.1,1.2,1.3,1.35,1.3,1.2,1.16};
    glColor3f(1.0,0.0,0.0);

    glBegin(GL_LINE_LOOP);
    for(int i=0; i<21;i++){
        glVertex3f(x[i],0.0,z[i]);
    }
    for(int i=20; i>=0;i--){
        glVertex3f(x[i]*-1,0.0,z[i]);
    }
    glEnd();
}

void drawSphereTurtle(void){
    int slices = 40;
    int stacks = 40;

    glPushMatrix();
    glScalef(1.0f, .3f, 1.0f);
    glutWireSphere(1.0, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.7f, 0.0f, .7f);
    glutWireSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.7f, 0.0f, .7f);
    glutWireSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.7f, 0.0f, -.7f);
    glutWireSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.7f, 0.0f, -.7f);
    glutWireSphere(.3, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glScalef(1.0f, .6f, 1.0f);
    glTranslatef(0.0f, 0.0f, -1.2f);
    glutWireSphere(.4, slices, stacks);
    glPopMatrix();
}

void axis(void){
    
    //Red, axix X
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(-1.0,0.0,0.0);
    //glEnd();

    //Green axis Y
    //glBegin(GL_LINE);
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,-1.0,0.0);
    //glEnd();

    //Blue axis Z
    //glBegin(GL_LINE);
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,-1.0);
    glEnd();
}

void display(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glColor3f(1.0,0.0,0.0);
    //glutWireTorus(0.25,0.75, 28, 28);
    //glColor3f(0.0,0.0,1.0) ;
    //glutWireCube(.60) ;
    if(flagTurtle){
        drawTurtle();
    }else{
        drawSphereTurtle();
    }
    if (flagAxis){
        axis();
    }
    glTranslatef(0.0,0.0,0.0);
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, 512, 512);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 128.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'h':
    case 'H':
        printf("help\n\n");
        printf("a - Axis\n");
        printf("1 - rotate y\n");
        printf("2 - rotate x\n");
        printf("3 - rotate z\n");
        printf("z - 2D/3D Turtle\n");
        printf("u - Move camera up\n");
        printf("d - Move camera down\n");
        printf("l - Move camera left\n");
        printf("r - Move camera right\n");
        printf("q/escape - Quit\n\n");
        break;
    case 'c':
    case 'C':
        if (glIsEnabled(GL_CULL_FACE))
           glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        break;
    case '1':
        glRotatef(1.0,1.0,0.0,0.0);
        break;
    case '2':
        glRotatef(1.0,0.0,1.0,0.0);
        break;
    case '3':
        glRotatef(1.0,0.0,0.0,1.0);
        break;
    case 'a':
    case 'A':
        flagAxis=!flagAxis;
        break;	
    case 'u':
    case 'U':
        Y -= 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,X,Y,0.0,0.0,1.0,0.0);
        break;
    case 'd':
    case 'D':
        Y += 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,X,Y,0.0,0.0,1.0,0.0);
        break;
    case 'l':
    case 'L':
        X += 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,X,Y,0.0,0.0,1.0,0.0);
        break;
    case 'r':
    case 'R':
        X -= 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,X,Y,0.0,0.0,1.0,0.0);
        break;
    case 'Q':
    case 'q':
        exit(0);
        break;
    case 'Z':
    case 'z':
        flagTurtle=!flagTurtle;
        break;
    }
    glutPostRedisplay();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("tecnunLogo");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    
    return 0;
}
