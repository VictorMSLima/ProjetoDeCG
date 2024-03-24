#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include "ImageLoader.h"

using namespace std;

float cameraX, cameraY, cameraZ;

GLfloat luz_pontual[] = {1.0, 1.0, 1.1, 0.7};


void desenhar_luz_pontual() {
    glPushAttrib(GL_LIGHTING_BIT);

    GLfloat light_diffuse[] = { 0.4, 0.4, 0.4, 1.0 }; 
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1.0 }; 
    GLfloat light_ambient[] = { 0.05, 0.05, 0.05, 1.0 }; 
    
    glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2f); 
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1f);

    glEnable(GL_LIGHT1);

    // Desenhar a esfera representando a luz
    glPushMatrix();
    glTranslatef(luz_pontual[0], luz_pontual[1], luz_pontual[2]);
    glColor3f(1.0, 1.0, 0.0); // Cor amarela
    glutSolidSphere(0.04, 50, 50);
    glPopMatrix();

}




void configurarLuzAmbiente() {


    GLfloat posicao_luz[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat luz_difusa[] = { 0.1, 0.1, 0.1, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);

    glEnable(GL_LIGHT0);

}



void iluminar()
{
    

    

    configurarLuzAmbiente();
    desenhar_luz_pontual();
    // glEnable(GL_LIGHT1);
}



void desenhar_eixos()
{
    // glPushMatrix();
    // glTranslated(0.0, 0.0, 0.0);

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

    // glPopMatrix();
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
            cameraX = ( cameraX * cos(-angulo) ) + ( cameraY * sin(-angulo) );
            cameraY = ( -cameraX * sin(-angulo) ) + ( cameraY * cos(-angulo) );
            break;
        case GLUT_KEY_RIGHT:
            cameraX = ( cameraX * cos(angulo) ) + ( cameraY * sin(angulo) );
            cameraY = ( -cameraX * sin(angulo) ) + ( cameraY * cos(angulo) );
            break;
        case GLUT_KEY_UP:
            cameraZ -= 0.1;
            break;
        case GLUT_KEY_DOWN:
            cameraZ += 0.1;
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

    gluLookAt(cameraX, cameraY, cameraZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    
    desenhar_eixos();
    iluminar(); // Esta função já configura ambas as fontes de luz

    const char *nomeArquivo = "imagem.pgm";
    Matriz matriz = ImageLoader().load(nomeArquivo);

    int largura = matriz.getLargura();
    int altura = matriz.getAltura();
    // Arrays para armazenar vértices e cores
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> cores;
    std::vector<GLfloat> normais;

    float eixoXnormal, eixoYnormal, eixoZnormal = 0.0;

    // Preencher os arrays de vértices e cores
    for (int i = 0; i < altura - 1; i++) {
        for (int j = 0; j < largura - 1; j++) {
            // Coordenadas e altura para o canto inferior esquerdo do quadrado
            float x1 = float(j) / float(largura);
            float y1 = float(i) / float(altura);
            float z1 = float(matriz.getValorPosicao(j, i)) / float(matriz.getValorMaximo());

            // Canto superior esquerdo
            float x2 = x1;
            float y2 = float(i+1) / float(altura);
            float z2 = float(matriz.getValorPosicao(j, i+1)) / float(matriz.getValorMaximo());

            // Canto inferior direito
            float x3 = float(j+1) / float(largura);
            float y3 = y1;
            float z3 = float(matriz.getValorPosicao(j+1, i)) / float(matriz.getValorMaximo());

            // Canto superior direito
            float x4 = x3;
            float y4 = y2;
            float z4 = float(matriz.getValorPosicao(j+1, i+1)) / float(matriz.getValorMaximo());

            // Primeiro triângulo (inferior esquerdo, superior esquerdo, superior direito)
            vertices.push_back(x1); vertices.push_back(y1); vertices.push_back(z1);
            vertices.push_back(x3); vertices.push_back(y3); vertices.push_back(z3);
            vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
            
            // Cores verde para o primeiro triângulo
            for (int k = 0; k < 3; k++) {
                cores.push_back(0.0); // R
                cores.push_back(1.0); // G
                cores.push_back(0.0); // B
            }
            
            // Calcular os vetores dos lados dos triângulos
            float lado1x = x3 - x1;
            float lado1y = y3 - y1;
            float lado1z = z3 - z1;
            float lado2x = x2 - x1;
            float lado2y = y2 - y1;
            float lado2z = z2 - z1;

            

            // Calcular o vetor normal do primeiro triângulo
            float nx1 = lado1y * lado2z - lado1z * lado2y;
            float ny1 = lado1z * lado2x - lado1x * lado2z;
            float nz1 = lado1x * lado2y - lado1y * lado2x;

            // Normalizar o vetor normal
            float comprimento1 = sqrt(nx1 * nx1 + ny1 * ny1 + nz1 * nz1);
            nx1 /= comprimento1;
            ny1 /= comprimento1;
            nz1 /= comprimento1;

            // Armazenar a normal para cada vértice do primeiro triângulo
            for (int k = 0; k < 3; k++) {
                normais.push_back(nx1);
                normais.push_back(ny1);
                normais.push_back(nz1);
            }
            
            // Segundo triângulo (inferior esquerdo, superior direito, inferior direito)
            vertices.push_back(x2); vertices.push_back(y2); vertices.push_back(z2);
            vertices.push_back(x3); vertices.push_back(y3); vertices.push_back(z3);
            vertices.push_back(x4); vertices.push_back(y4); vertices.push_back(z4);
            

            // Cores vermelho para o segundo triângulo
            for (int k = 0; k < 3; k++) {
                cores.push_back(0.0); // R
                cores.push_back(1.0); // G
                cores.push_back(0.0); // B
            }

            float lado3x = x2 - x4;
            float lado3y = y2 - y4;
            float lado3z = z2 - z4;

            float lado4x = x3 - x4;
            float lado4y = y3 - y4;
            float lado4z = z3 - z4;
            // Calcular a normal do segundo triângulo
            float nx2 = lado3y * lado4z - lado3z * lado4y;
            float ny2 = lado3z * lado4x - lado3x * lado4z;
            float nz2 = lado3x * lado4y - lado3y * lado4x;
            // Normalizar a normal do segundo triângulo
            float norma2 = sqrt(nx2 * nx2 + ny2 * ny2 + nz2 * nz2);
            nx2 /= norma2;
            ny2 /= norma2;
            nz2 /= norma2;

            // Armazenar a normal para cada vértice do segundo triângulo
            for (int k = 0; k < 3; k++) {
                normais.push_back(nx2);
                normais.push_back(ny2);
                normais.push_back(nz2);
            }
        }
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);


    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glColorPointer(3, GL_FLOAT, 0, &cores[0]);
    glNormalPointer(GL_FLOAT, 0, &normais[0]);

    
    glPushAttrib(GL_LIGHTING_BIT);
    glEnable(GL_LIGHTING); // Habilita a iluminação antes de desenhar os triângulos

    // Configuração do material
    GLfloat mat_difuso[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat mat_ambiente[] = {0.2, 1.0, 0.2, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_difuso);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambiente);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    glDisable(GL_LIGHTING); // Desabilita a iluminação após desenhar os triângulos

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glNormalPointer(GL_FLOAT, 0, &normais[0]);


    glPopAttrib();

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
