//#include <stdarg.h>
#include<GL/freeglut.h>
#include <GL/glut.h>
#include<fstream>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Camara.h"
#include "primitivas.h"
#include "Vector_tools.h"

#define PI 3,14159265358979323846

using namespace std;

bool command = false; /* command mode */
char strCommand[256];

static Camara *MiCamara;
static int spot_move = 0;
static int old_x, old_y;
bool showTurtle = true;
bool flagAxis = false;
bool flagTurtle = true;
int giroCompleto;
ifstream fe;

void axis(void){
    
    glPushMatrix();
    glLoadIdentity();
    //Red, axix X
    glBegin(GL_LINES);
    glColor3f(1.0,0.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(-1.0,0.0,0.0);

    //Green axis Y
    glColor3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,-1.0,0.0);

    //Blue axis Z
    glColor3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,1.0);
    glVertex3f(0.0,0.0,-1.0);
    glEnd();

    glPopMatrix();
}

void display(void) {
    float At[3];
    float Direction[3];
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHTING);
    
    MiCamara->SetGLCamera();
    
    if(showTurtle){
        glPushMatrix();
        drawSphereTurtle();
        glPopMatrix();
    }
    
    if(flagAxis){
        DrawAxis();
        axis();
    }
    
    glutSwapBuffers();
}

float ConversorAngulo(float angulo) {
    switch (giroCompleto) {
        case 400:
            return (angulo * 360) / 400;
        case 12:
        case 60:
            return (angulo * 360) / 60;
        case 2:
            return (angulo * 180);
    }
}   

void load(char* archivo) {
    fe.open(archivo);
}

char * insideRepeat(char* strCommandInside) {
    char *ini, *fin;
    ini = strchr(strCommandInside,'[');
    if (ini == NULL) return NULL;
        ini++;
        fin = strrchr(strCommandInside,']');
    if (fin == NULL) return NULL;
        strCommandInside[fin-strCommandInside]=0;
    return ini;
}

void parseCommand(char* strCommandParse) {
    char *strToken0;
    char *strToken1;
    char *repeatCommand;
    char *nextCommand;
    char parseCommandInit[256];
    int i;
    double val;
    strToken0 = strtok(strCommandParse, " ");
    while ((strToken1 = strtok(NULL, " ")) != NULL) {
        val = atof(strToken1);
        if (!strcmp("repeat",strToken0)) {
            repeatCommand = insideRepeat(strToken1 + strlen(strToken1) + 1);
        if (repeatCommand == NULL) return;
            nextCommand = repeatCommand + strlen(repeatCommand) + 1;
        for (i = 0; i < val; i++) {
            strcpy (parseCommandInit, repeatCommand);
            parseCommand(parseCommandInit);
        }
            strToken0 = strtok(nextCommand, " ");
        if (strToken0 == NULL) continue;
        continue;
        } 
        if (!strcmp("load", strToken0)) {
            printf("-----CARGO----");

            load(strToken1);
            while (!fe.eof()) {
                fe.getline(strCommand, 128);
                strcat(strCommand, " ");
                parseCommand(strCommand);
                strcpy(strCommand, "");

            }
            fe.close();
        } else {
            val = atof(strToken1);
            if (!strcmp("sc", strToken0)) { // DOWN
                if (strToken1 == "2pi" || strToken1 == "2PI") {
                    giroCompleto = 2;
                } else {
                    giroCompleto = val;
                }
            } else if (!strcmp("fd", strToken0)) { // FORWARD
                glTranslatef(0.0, 0.0, val);
            } else if (!strcmp("bk", strToken0)) { // BACK
                glTranslatef(0.0, 0.0, -val);
            } else if (!strcmp("rt", strToken0)) { // RIGHT
                glRotatef(-val, 0., 1., 0.);
            } else if (!strcmp("lt", strToken0)) { // LEFT
                glRotatef(val, 0., 1., 0.);
            } else if (!strcmp("up", strToken0)) { // UP
                glRotatef(val, 1., 0., 0.);
            } else if (!strcmp("dn", strToken0)) { // DOWN
                glRotatef(-val, 0., 0., 1.);
            } else if (!strcmp("rr", strToken0)) { // UP
                glRotatef(val, 1., 0., 0.);
            } else if (!strcmp("lr", strToken0)) { // DOWN
                glRotatef(-val, 0., 0., 1.);
            } else if (!strcmp("sx", strToken0)) { // DOWN
                glScalef(val, 1., 1.);
            } else if (!strcmp("sy", strToken0)) { // UP
                glScalef(1., val, 1.);
            } else if (!strcmp("sz", strToken0)) { // DOWN
                glScalef(1., 1., val);
            } else if (!strcmp("ht", strToken0)) { // UP
                showTurtle = false;
            } else if (!strcmp("st", strToken0)) { // DOWN
                showTurtle = true;
            } else {
                break;
            }
        strToken0 = strtok(NULL, " ");
        display();
        }
        // EXIT COMMAND MODE
        if (strToken0 != NULL && strncmp(strToken0, "exit", 4) == 0) {
            command = false;
            // HOME
        } else if (strToken0 != NULL && !strcmp("home", strToken0)) {
            glLoadIdentity();
        }
    }
}

void reshape(int width, int height) {
    int dim = (width < height) ? width : height;
    glViewport(0, 0, dim, dim);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat) dim / (GLfloat) dim, 1.0, 128.0);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

void MouseMotion(int x, int y) {
    old_y = y;
    old_x = x;
}

void Zoom(int x, int y) {

    float zoom;

    zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
    old_y = y;

    switch (MiCamara->camMovimiento) {

        case CAM_EXAMINAR:
            if (MiCamara->camAperture + zoom > (5 * DEGREE_TO_RAD) && MiCamara->camAperture + zoom < 175 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture + zoom;
            break;

    }

    glutPostRedisplay();
}

void Examinar(int x, int y) {

    float rot_x, rot_y;

    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);
    MiCamara->Rotar_Latitud( rot_y * DEGREE_TO_RAD);
    MiCamara->Rotar_Longitud(rot_x * DEGREE_TO_RAD);

    old_y = y;
    old_x = x;

    glutPostRedisplay();

}

/*
void Mouse_Spot_Abrir_Cerrar(int x, int y) {

    float step;

    step = (float) (y - old_y);
    old_y = y;

    if (LOCAL_MyLights[current_light]->spotCutOff + step < 90 && LOCAL_MyLights[current_light]->spotCutOff + step > 0)
        LOCAL_MyLights[current_light]->spotCutOff += step;

    LOCAL_MyLights[current_light]->needsUpdate = TRUE;
    glutPostRedisplay();
}
void Mouse_Spot(int x, int y) {
    float rot_x, rot_y;

    rot_y = (float) (old_y - y);
    rot_x = (float) (x - old_x);

    Rotar_Spot_Latitud(LOCAL_MyLights[current_light], rot_y * DEGREE_TO_RAD);
    Rotar_Spot_Longitud(LOCAL_MyLights[current_light], rot_x * DEGREE_TO_RAD);

    old_y = y;
    old_x = x;

    glutPostRedisplay();
}*/

void Andar(int x, int y) {

    float rotacion_x, avance_y;

    avance_y = (float) (y - old_y) / 10;
    rotacion_x = (float) (old_x - x) * DEGREE_TO_RAD / 5;
    MiCamara->YawCamera(rotacion_x);
    MiCamara->AvanceFreeCamera(avance_y);

    old_y = y;
    old_x = x;

    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {

    old_x = x;
    old_y = y;

    switch (button) {

        case GLUT_LEFT_BUTTON:

            switch (MiCamara->camMovimiento) {

                case CAM_EXAMINAR:
                    if (state == GLUT_DOWN)
                        glutMotionFunc(Zoom);
                    if (state == GLUT_UP) {
                        glutPassiveMotionFunc(Examinar);
                        glutMotionFunc(NULL);
                    }
                    break;

                case CAM_PASEAR:
                    if (state == GLUT_DOWN)
                        glutMotionFunc(Andar);
                    if (state == GLUT_UP)
                        glutMotionFunc(NULL);
                    break;
            }

        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                //glutMotionFunc(scale);
                break;

        default:
            break;

    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (command) {
        if (key == 13) {
            strcat(strCommand, " ");
            if (strlen(strCommand) == 1) command = false;
            parseCommand(strCommand);
            strcpy(strCommand, "");
        } else {
            char strKey[2] = " ";
            strKey[0] = key;
            printf(strKey);
            strcat(strCommand, strKey);
        }
    } else { // not in command mode
        switch (key) {
            case 'H':
            case 'h':
                printf("help\n\n");
                printf("c - Toggle culling\n");
                printf("q/escape - Quit\n\n");
                break;
            case 'C':
            case 'c':
                if (glIsEnabled(GL_CULL_FACE))
                    glDisable(GL_CULL_FACE);
                else
                    glEnable(GL_CULL_FACE);
                break;
            case 'a':
            case 'A':
                flagAxis=!flagAxis;
                break;	
            case '1':
                glRotatef(1.0, 1., 0., 0.);
                break;
            case '2':
                glRotatef(1.0, 0., 1., 0.);
                break;
            case '3':
                glRotatef(1.0, 0., 0., 1.);
            case '4':
                glRotatef(1.0, -1., 0., 0.);
                break;
            case '5':
                glRotatef(1.0, 0., -1., 0.);
                break;
            case '6':
                glRotatef(1.0, 0., 0., -1.);
                break;
            case 'I':
            case 'i':
                command = true;
                break;
            case 'Q':
            case 'q':
            case 27:
                delete MiCamara;
                exit(0);
                break;
        }
    }
    glutPostRedisplay();
}

static void SpecialKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            glutPassiveMotionFunc(MouseMotion);
            MiCamara->camMovimiento = CAM_STOP;
            break;
        case GLUT_KEY_F2:
            glutPassiveMotionFunc(Examinar);
            MiCamara->camMovimiento = CAM_EXAMINAR;
            break;
        case GLUT_KEY_F3:
            glutPassiveMotionFunc(MouseMotion);
            MiCamara->camMovimiento = CAM_PASEAR;
            MiCamara->camAtY = 0;
            MiCamara->camViewY = 0;
            MiCamara->SetDependentParametersCamera();
            break;

        case GLUT_KEY_PAGE_UP:
            //AvanceFreeCamera( MiCamara, 0.5f );
            if (MiCamara->camAperture < 175 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture + 2.5f * DEGREE_TO_RAD;
            break;

        case GLUT_KEY_PAGE_DOWN:
            //AvanceFreeCamera( MiCamara, -0.5f );
            if (MiCamara->camAperture > 5 * DEGREE_TO_RAD)
                MiCamara->camAperture = MiCamara->camAperture - 2.5f * DEGREE_TO_RAD;
            break;

        case GLUT_KEY_UP:
            MiCamara->Rotar_Latitud( 2.5f * DEGREE_TO_RAD);
            //PitchCamera( MiCamara, 3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_DOWN:
            MiCamara->Rotar_Latitud( -2.5f * DEGREE_TO_RAD);
            //PitchCamera( MiCamara, -3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_LEFT:
            MiCamara->Rotar_Longitud( -2.5f * DEGREE_TO_RAD);
            //YawCamera( MiCamara, 3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_RIGHT:
            MiCamara->Rotar_Longitud(2.5f * DEGREE_TO_RAD);
            //YawCamera( MiCamara, -3.0f * DEGREE_TO_RAD );
            break;

        case GLUT_KEY_HOME: //Reset Camera
            MiCamara->camAtX = 0;
            MiCamara->camAtY = 0;
            MiCamara->camAtZ = 0;
            MiCamara->camViewX = 0;
            MiCamara->camViewY = 1;
            MiCamara->camViewZ = -3;
            MiCamara->SetDependentParametersCamera();
            break;
        default:
            printf("key %d %c X %d Y %d\n", key, key, x, y);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    // Colocamos la cámara en (0,1,-3) mirando hacia (0,0,0)
    MiCamara = new Camara(0, 1, -3);

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("tortuga");
    //glEnable(GL_COLOR_MATERIAL);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKey);

    //Ratón
    glutMouseFunc(mouse); //Pulsado de Botones
    glutMotionFunc(NULL); //Movimiento con Botones pulsados
    glutPassiveMotionFunc(MouseMotion); //Movimientos sin Botones pulsados

    glutMainLoop();
    return 0;
}