#define GL_SILENCE_DEPRECATION

#ifdef _WIN32
#include "glut.h"
#include "GL.h"
#include "GLAUX.H"
#include "GLU.h"
#elif __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // 이미지 파일 불러오기

#include <iostream>
#define PI 3.14

using namespace std;

float SCR_WIDTH, SCR_HEIGHT;

GLuint currentTexture = 0;
GLuint textures[6];

bool isMousePushed;
GLdouble startX, startY, startRH, startRV;

GLdouble zoom = 2;
GLdouble radianV = 0;
GLdouble radianH = 90;

GLfloat AmbientLightValue[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat DiffuseLightValue[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat SpecularLightValue[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat PositionLightValue[] = { 3, 5, 3, 0.0f };

GLfloat mat_ambdif[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_spc[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shn[] = { 10.0 };

GLfloat r_mat_ambdif[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat g_mat_ambdif[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat b_mat_ambdif[] = { 0.0, 0.0, 1.0, 1.0 };

void DrawBox(int texture, float texture_start[], float size[], float translate[], float scale) {

    float box_left = texture_start[0];
    float box_top = texture_start[1];

    float size_x = size[0];
    float size_y = size[1];
    float size_z = size[2];

    glPushMatrix();
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, texture);

    glTranslatef(translate[0], translate[1], translate[2]);
    glScalef(size_x * scale, size_y * scale, size_z * scale);

    glBegin(GL_QUADS);

    // Front Face
    float left = box_left + size_z;
    float top = box_top + size_z;
    float right = left + size_x;
    float bottom = top + size_y;

    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(left, bottom);
    glVertex3f(-1.0f, -1.0f, 1.0f);   // 좌하단
    glTexCoord2f(right, bottom);
    glVertex3f(1.0f, -1.0f, 1.0f);    // 우하단
    glTexCoord2f(right, top);
    glVertex3f(1.0f, 1.0f, 1.0f);     // 우상단
    glTexCoord2f(left, top);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // 좌상단

    // Back Face
    left = box_left + size_z + size_x + size_z;
    top = box_top + size_z;
    right = left + size_x;
    bottom = top + size_y;

    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(left, bottom);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(right, bottom);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(right, top);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(left, top);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Top Face
    left = box_left + size_z;
    top = box_top;
    right = left + size_x;
    bottom = top + size_z;

    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(left, bottom);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(right, bottom);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(right, top);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(left, top);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Down Face
    left = box_left + size_z + size_x;
    top = box_top;
    right = left + size_x;
    bottom = top + size_z;

    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(left, bottom);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(left, top);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(right, top);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(right, bottom);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right face
    left = box_left + size_z + size_x;
    top = box_top + size_z;
    right = left + size_z;
    bottom = top + size_y;

    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(left, bottom);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(right, bottom);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(right, top);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(left, top);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Left Face
    left = box_left;
    top = box_top + size_z;
    right = left + size_z;
    bottom = top + size_y;

    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(left, bottom);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(right, bottom);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(right, top);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(left, top);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

void DrawHuman() {

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, SCR_WIDTH / SCR_HEIGHT, 1.0, 2000);
    gluLookAt(
        zoom * cos(radianH * PI / 180),
        zoom * sin(radianV * PI / 180) + 0.25,
        zoom * sin(radianH * PI / 180),
        0, 0.25, 0, 
        0, 1, 0);
        
    glMatrixMode(GL_MODELVIEW);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambdif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_ambdif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spc);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shn);

    float texture_start[2];
    float size[3];
    float translate[3];

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ HEAD

    texture_start[0] = (float)0;
    texture_start[1] = (float)0;
    size[0] = (float)1 / 8;
    size[1] = (float)1 / 8;
    size[2] = (float)1 / 8;
    translate[0] = (float)0;
    translate[1] = (float)1 / 2;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ BODY

    texture_start[0] = (float)1 / 4;
    texture_start[1] = (float)1 / 4;
    size[0] = (float)1 / 8;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)0;
    translate[1] = (float)3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LEFT ARM

    texture_start[0] = (float)5 / 8;
    texture_start[1] = (float)1 / 4;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)-3 / 16;
    translate[1] = (float)3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ RIGHT ARM

    texture_start[0] = (float)1 / 2;
    texture_start[1] = (float)3 / 4;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)3 / 16;
    translate[1] = (float)3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LEFT LEG

    texture_start[0] = (float)0;
    texture_start[1] = (float)1 / 4;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)-1 / 16;
    translate[1] = (float)-3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ RIGHT LEG

    texture_start[0] = (float)1 / 4;
    texture_start[1] = (float)3 / 4;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)1 / 16;
    translate[1] = (float)-3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ HEAD OVERLAY

    texture_start[0] = (float)1 / 2;
    texture_start[1] = (float)0;
    size[0] = (float)1 / 8;
    size[1] = (float)1 / 8;
    size[2] = (float)1 / 8;
    translate[0] = (float)0;
    translate[1] = (float)1 / 2;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1.1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ BODY OVERLAY

    texture_start[0] = (float)1 / 4;
    texture_start[1] = (float)1 / 2;
    size[0] = (float)1 / 8;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)0;
    translate[1] = (float)3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1.08);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LEFT ARM OVERLAY

    texture_start[0] = (float)5 / 8;
    texture_start[1] = (float)1 / 2;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)-3 / 16;
    translate[1] = (float)3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1.1);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ RIGHT ARM OVERLAY

    texture_start[0] = (float)3 / 4;
    texture_start[1] = (float)3 / 4;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)3 / 16;
    translate[1] = (float)3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1.11);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LEFT LEG OVERLAY

    texture_start[0] = (float)0;
    texture_start[1] = (float)1 / 2;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)-1 / 16;
    translate[1] = (float)-3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1.11);

    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ RIGHT LEG OVERLAY

    texture_start[0] = (float)0;
    texture_start[1] = (float)3 / 4;
    size[0] = (float)1 / 16;
    size[1] = (float)3 / 16;
    size[2] = (float)1 / 16;
    translate[0] = (float)1 / 16;
    translate[1] = (float)-3 / 16;
    translate[2] = (float)0;

    DrawBox(currentTexture, texture_start, size, translate, 1.09);

}

void DrawAxis() {

    glViewport(0, 0, 150, 150);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-20, 20, -20, 20, -20, 20);

    gluLookAt(
        10 * cos(radianH * PI / 180),
        10 * sin(radianV * PI / 180),
        10 * sin(radianH * PI / 180),
        0, 0, 0, 0, 1, 0);

    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D, -1);

    glPushMatrix();
    glLoadIdentity();

    glLineWidth(10.0);
    glBegin(GL_LINE_STRIP);
    glNormal3f(1, 0.5, 0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, r_mat_ambdif);
    glVertex3f(0.0, 0, 0.0);
    glVertex3f(10.0, 0, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, g_mat_ambdif);
    glVertex3f(0.0, 0, 0.0);
    glVertex3f(0.0, 10, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, b_mat_ambdif);
    glVertex3f(0.0, 0, 0.0);
    glVertex3f(0.0, 0, 10.0);
    glEnd();

    glPopMatrix();
}

void Display() {

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();

    DrawHuman();
    DrawAxis();

    glFlush();
    glutSwapBuffers();
}

void Reshape(int w, int h) {
    SCR_WIDTH = w;
    SCR_HEIGHT = h;

    glutPostRedisplay();
}

void MouseClick(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        isMousePushed = true;
        startX = x;
        startY = y;
        startRH = radianH;
        startRV = radianV;
    }
    else 
        isMousePushed = false;

    glutPostRedisplay();
}

void MouseMotion(int x, int y) {

    if (isMousePushed) {
        radianH = startRH - (startX - x) / 5;
        radianV = startRV - (startY - y) / 5;
    }

    if (radianV > 80) radianV = 80;
    if (radianV < -80) radianV = -80;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {

    if (key == 'a') // zoom in
        zoom -= 0.1;
    else if (key == 'z') // zoom out
        zoom += 0.1;

    if (key >= '1' && key <= '9')
        currentTexture = textures[key - '1'];

    glutPostRedisplay();
}

void load_image(const char* file, int number) {

    cout << "load " << file << endl;

    int width, height, nrChannels;

    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    glBindTexture(GL_TEXTURE_2D, textures[number]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    SCR_WIDTH = 720;
    SCR_HEIGHT = 1280;

    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutCreateWindow("3D VIrtual Human Modeling");

    glClearColor(0.3, 0.3, 0.3, 1.0f);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE); //가려진 부분은 그려지지 않게 한다.

    // light setting
    glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLightValue); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLightValue); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLightValue); 
    glLightfv(GL_LIGHT0, GL_POSITION, PositionLightValue); 

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    // texture setting
    glGenTextures(6, textures);

    load_image("steve.png", 0);
    load_image("debug.png", 1);
    load_image("Gawr_Gura.png", 2);
    load_image("usada_pekora.png", 3);
    load_image("RGB.png", 4);
    load_image("hacker.png", 5);

    glEnable(GL_TEXTURE_2D);
    currentTexture = textures[0];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMotion);

    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;

}
