#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include "ImageLoader.h"

using namespace std;

float cameraX, cameraY, cameraZ;

GLfloat luz_pontual[] = {1.0, 0.0, 1.0, 0.0};

void iluminar()
{
    GLfloat posicao_luz[] = { 0.0, 1.0, 0.0, 0.0 };
    GLfloat luz_difusa[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);

    GLfloat light1_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light1_specular[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat light1_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient); 

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void desenhar_luz(){

   glPushAttrib (GL_LIGHTING_BIT);

   GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };


   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

   glPushMatrix();
   glTranslatef(luz_pontual[0],luz_pontual[1],luz_pontual[2]);

   glEnable(GL_LIGHTING);
   glColor3f (1.0, 1.0, 0.0);
   glutSolidSphere(0.1,50,50);
   glDisable(GL_LIGHTING);
   
   glPopAttrib();
   glPopMatrix();
}

void desenhar_eixos()
{
    glPushMatrix();
    glTranslated(0.0, -0.5, 0.0);

    glBegin(GL_LINES);
        // Eixo x
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(1.0, 0.0, 0.0);

        // Eixo y
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);

        // Eixo z
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    glPopMatrix();
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0.0);

    iluminar();

    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade
    glShadeModel(GL_SMOOTH); // Estabelece o modelo de coloração como interpolação entre os vértices

    cameraX = 0.3;
    cameraY = 0.3;
    cameraZ = 0.3;
}

void specialKeys(int key, int x, int y)
{
    float angulo = (2 * M_PI) / 180;

    switch(key)
    {
        case GLUT_KEY_LEFT:
            cameraX = ( cameraX * cos(-angulo) ) + ( cameraZ * sin(-angulo) );
            cameraZ = ( -cameraX * sin(-angulo) ) + ( cameraZ * cos(-angulo) );
            break;
        case GLUT_KEY_RIGHT:
            cameraX = ( cameraX * cos(angulo) ) + ( cameraZ * sin(angulo) );
            cameraZ = ( -cameraX * sin(angulo) ) + ( cameraZ * cos(angulo) );
            break;
        case GLUT_KEY_UP:
            cameraY -= 0.1;
            break;
        case GLUT_KEY_DOWN:
            cameraY += 0.1;
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1.0, 1.0, -1.0, 1.0, -20.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);

    desenhar_luz();

    desenhar_eixos();

    glPushMatrix();
    glTranslated(0.0, -0.5, 0.0);

    glPushAttrib(GL_LIGHTING_BIT);
    
    GLfloat mat_specular[] = { 0.0, 0.5, 0.5, 1.0 };
    GLfloat mat_diffuse[] = { 0.0, 0.5, 0.5, 0.0 };
    GLfloat mat_shininess[] = { 10.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glEnable(GL_LIGHTING);
    
    const char *nomeArquivo = "imagem.pgm";
    Matriz matriz = ImageLoader().load(nomeArquivo);

    float eixoX, eixoY, eixoZ = 0.0;
    float eixoX1, eixoY1, eixoZ1 = 0.0;
    float eixoX2, eixoY2, eixoZ2 = 0.0;
    float eixoXnormal, eixoYnormal, eixoZnormal = 0.0;

    for(int x = 0; x < matriz.getAltura() - 1; x++)
    {
        for(int y = 0; y < matriz.getLargura() - 1; y++)
        {
            glBegin(GL_TRIANGLES);
                eixoX = (float) x / (float) matriz.getLargura();
                eixoY = (float) y / (float) matriz.getAltura();
                eixoZ = (float) matriz.getValorPosicao(x, y) / (float) matriz.getValorMaximo();

                eixoX1 = (float) x / (float) matriz.getLargura();
                eixoY1 = (float) (y + 1) / (float) matriz.getAltura();
                eixoZ1 = (float) matriz.getValorPosicao(x, y + 1) / (float) matriz.getValorMaximo();

                eixoX2 = (float) (x + 1) / (float) matriz.getLargura();
                eixoY2 = (float) y / (float) matriz.getAltura();
                eixoZ2 = (float) matriz.getValorPosicao(x + 1, y) / (float) matriz.getValorMaximo();

                eixoXnormal = (eixoX1 + eixoX2 + eixoX) / 3;
                eixoYnormal = (eixoY1 + eixoY2 + eixoY) / 3;
                eixoZnormal = (eixoZ1 + eixoZ2 + eixoZ) / 3;
                
                glNormal3f(eixoXnormal, eixoZnormal, eixoYnormal);
                glVertex3f(eixoX, eixoZ, eixoY);
                glVertex3f(eixoX1, eixoZ1, eixoY1);
                glVertex3f(eixoX2, eixoZ2, eixoY2);

                eixoX = (float) (x + 1) / (float) matriz.getLargura();
                eixoY = (float) y / (float) matriz.getAltura();
                eixoZ = (float) matriz.getValorPosicao(x + 1, y) / (float) matriz.getValorMaximo();

                eixoX1 = (float) x / (float) matriz.getLargura();
                eixoY1 = (float) (y + 1) / (float) matriz.getAltura();
                eixoZ1 = (float) matriz.getValorPosicao(x, y + 1) / (float) matriz.getValorMaximo();

                eixoX2 = (float) (x + 1) / (float) matriz.getLargura();
                eixoY2 = (float) (y + 1) / (float) matriz.getAltura();
                eixoZ2 = (float) matriz.getValorPosicao(x + 1, y + 1) / (float) matriz.getValorMaximo();

                eixoXnormal = (eixoX1 + eixoX2 + eixoX) / 3;
                eixoYnormal = (eixoY1 + eixoY2 + eixoY) / 3;
                eixoZnormal = (eixoZ1 + eixoZ2 + eixoZ) / 3;
                
                glNormal3f(eixoXnormal, eixoZnormal, eixoYnormal);
                glVertex3f(eixoX, eixoZ, eixoY);
                glVertex3f(eixoX1, eixoZ1, eixoY1);
                glVertex3f(eixoX2, eixoZ2, eixoY2);

            glEnd();
        }
    }

    glDisable(GL_LIGHTING);

    glPopAttrib();

    glPopMatrix();

    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(350, 350);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Eu, Carrinho!");

    init();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
