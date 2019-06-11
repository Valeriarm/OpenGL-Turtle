/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: javargas
 *
 * Created on February 12, 2017, 11:49 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>


using namespace std;
bool flagAxis = false;
GLfloat X=0, Y=1.0 ,Z=3.0;

void drawTurtle(void){
    double x[]={0.0,-0.1,-0.3,-0.1,-0.1,-0.4,-0.45,-0.5,-0.55,-0.6,-045},
           z[]={0.0,-0.1,-0.3,-0.3,-0.45,-0.45,-0.4,-0.38,-0.4,-0.5,-0.5};
    glColor3f(1.0,0.0,0.0);

    glBegin(GL_LINE_LOOP);
    for(int i=0; i<11;i++){
        glVertex3f(x[i],0.0,z[i]);
    }
    for(int i=10; i>=0;i--){
        glVertex3f(x[i]*-1,0.0,z[i]);
    }
    glEnd();
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
    drawTurtle();
    if (flagAxis){
        axis();
    }
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
    cout << "I am a key:" << key << '\n';
    switch (key) {
    case 'h':
        printf("help\n\n");
        printf("c - Toggle culling\n");
        printf("a - Axis\n");
        printf("q/escape - Quit\n\n");
        break;
    case 'c':
        if (glIsEnabled(GL_CULL_FACE))
           glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);
        break;
    case '1':
        glRotatef(1.0,1.,0.,0.);
        break;
    case '2':
        glRotatef(1.0,0.,1.,0.);
        break;
    case 'a':
        flagAxis=!flagAxis;
        break;	
    case 'u':
        Y += 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,0.0,0.0,0.0,0.0,1.0,0.0);
        break;
    case 'd':
        Y -= 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,0.0,0.0,0.0,0.0,1.0,0.0);
        break;
    case 'l':
        X -= 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,0.0,0.0,0.0,0.0,1.0,0.0);
        break;
    case 'r':
        X += 0.1f;
        glLoadIdentity();
        gluLookAt(X,Y,Z,0.0,0.0,0.0,0.0,1.0,0.0);
        break;
    case 'q':
    case 27:
        exit(0);
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