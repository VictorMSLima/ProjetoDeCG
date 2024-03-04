#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include "ImageLoader.h"

using namespace std;

float posCameraX,posCameraY,posCameraZ;

void init(void) 
{
  glClearColor (1.0, 1.0, 1.0, 0.0);
  
  posCameraX = 0.3;
  posCameraY = 0.3;
  posCameraZ = 0.3;
}

void specialKeys(int key, int x, int y)
{
  float angulo = 2*M_PI/180;
  switch (key) {
    case GLUT_KEY_LEFT : 
      posCameraX =  posCameraX*cos(-angulo) + posCameraY*sin(-angulo);
      posCameraY = -posCameraX*sin(-angulo) + posCameraY*cos(-angulo);
      break;
    case GLUT_KEY_RIGHT : 
      posCameraX =  posCameraX*cos(angulo) + posCameraY*sin(angulo);
      posCameraY = -posCameraX*sin(angulo) + posCameraY*cos(angulo);                      
      break;
    case GLUT_KEY_UP:
      posCameraZ += 0.01;
      break;
    case GLUT_KEY_DOWN:
      posCameraZ -= 0.01;
      break;
  }
  glutPostRedisplay();
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt (posCameraX, posCameraY, posCameraZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

  const char *nomeArquivo = "imagem.pgm";

  ImageLoader loader;
  Matriz matriz = loader.load(nomeArquivo);

  const vector<vector<int>> &pixels = matriz.getMatriz();

  glBegin(GL_LINES);
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    
    glColor3f (0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
  
    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);    
  glEnd();
  
  float eixoX = 0.0;
  float eixoY = 0.0;
  float eixoZ = 0.0;

  glColor3f(0.7, 0.5, 0.4);
  for(int x = 0; x < matriz.getLargura() - 1; x++)
  {
    for(int y = 0; y < matriz.getAltura() - 1; y++)
    {
      glBegin(GL_LINE_LOOP);
        eixoX = (float) x / (float) matriz.getLargura();
        eixoY = (float) y / (float) matriz.getAltura();
        eixoZ = (float) matriz.getValorPosicao(x, y) / (float) matriz.getValorMaximo();

        glVertex3f (eixoX, eixoY, eixoZ);

        eixoX = (float) (x + 1) / (float) matriz.getLargura();
        eixoY = (float) y / (float) matriz.getAltura();
        eixoZ = (float) matriz.getValorPosicao(x + 1, y) / (float) matriz.getValorMaximo();
        
        glVertex3f (eixoX, eixoY, eixoZ);

        eixoX = (float) x / (float) matriz.getLargura();
        eixoY = (float) (y + 1) / (float) matriz.getAltura();
        eixoZ = (float) matriz.getValorPosicao(x, y + 1) / (float) matriz.getValorMaximo();
        
        glVertex3f (eixoX, eixoY, eixoZ);
      glEnd();
    }
  }

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);

  init ();

  glutDisplayFunc(display); 
  glutSpecialFunc(specialKeys);
  glutReshapeFunc(reshape);
  
  glutMainLoop();
  
  return 0;
}
