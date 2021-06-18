#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#define GL_PI 3.1416f

GLint opcion = 0;
void EscalaVentana(GLsizei w, GLsizei h)
{

 // Evita una división por cero
 if (h == 0) h = 1;

 printf("altura: %d \n",h);
 printf("anchura: %d \n",w);

 //glViewport(0, 0, w, h);

 // Ajusta la vista a las dimensiones de la ventana para no deformar la figura

 if (w > h) {
 glViewport (0, 0, (GLsizei) h, (GLsizei) h);
 }else{
 glViewport (0, 0, (GLsizei) w, (GLsizei) w);
 }

 // Reinicia el sistema de coordenadas en la matriz de PROYECCION
 glMatrixMode (GL_PROJECTION);
 // Se carga la matriz identidad en la matriz de PROYECCION
 glLoadIdentity();
 // Establece el volumen de trabajo ORTONORMAL
 glOrtho(-7.0f, 7.0f, -7.0f, 7.0f, -7.0, 7.0);
 // Se activa la matriz MODELO VISTA, que afecta a la primitivas geometricas
 glMatrixMode (GL_MODELVIEW);
 glFlush();
}


void DibujaTela(){

 glClearColor (0., 0., 1., 1.); // color de borrar
 glClear(GL_COLOR_BUFFER_BIT); // borramos pantalla
 glColor3f(1.0, 0.0, 0.0);// Pone el color de pintar a rojo

 glPointSize(2.0); // Indica el tamaño del punto

 glLineWidth(1.0); // Indica el groso de la linea


 GLUquadricObj *quadobj;
 quadobj = gluNewQuadric();
 gluQuadricDrawStyle( quadobj,GLU_LINE);
 gluDisk(quadobj,0,10,36,50);

}

void caracol(){
GLfloat ang, x, y;
GLfloat radio = 0.8f;
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_POINTS);
for (ang = 0.0f; ang < 2 * GL_PI; ang += 2*GL_PI/20000){
    x =  (2 + 4*sin(ang))*cos(ang);
    y =  (2 + 4*sin(ang))*sin(ang);
    glVertex2f(x,y);
}
glEnd();
}

void Espiral(){
GLfloat ang, x, y;
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_POINTS);
for (ang = 0.0f; ang < 6 * GL_PI; ang += 2*GL_PI/20000){
    x =  cos(ang)*ang;
    y =  sin(ang)*ang;
    glVertex2f(x,y);
}
glEnd();
}

void rosal(){
GLfloat ang, x, y;
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_POINTS);
for (ang = 0.0f; ang < 2 * GL_PI; ang += 2*GL_PI/20000){
    x =  3*cos(2*ang)*cos(ang);
    y =  3*cos(2*ang)*sin(ang);
    glVertex2f(x,y);
}
glEnd();
}

void cardioide(){
GLfloat ang, x, y;
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_POINTS);
for (ang = 0.0f; ang < 2 * GL_PI; ang += 2*GL_PI/20000){
    x =  (1.5 - 1.5*sin(ang))*cos(ang);
    y =  (1.5 - 1.5*sin(ang))*sin(ang);
    glVertex2f(x,y);
}
glEnd();
}

void circunferencia(){
GLfloat ang, x, y;
GLfloat radio = 0.8f;
glColor3f(1.0, 1.0, 0.0);
glBegin(GL_POINTS);
for (ang = 0.0f; ang < 2 * GL_PI; ang += 2*GL_PI/2000){
    x = radio * sin(ang);
    y = radio * cos(ang);
        glVertex2f(x,y);
}
glEnd();
}

void DibujaEscena(){

 DibujaTela();

 switch (opcion) {
 case 1: circunferencia(); break;
 case 2: caracol(); break;
 case 3: rosal(); break;
 case 4: cardioide(); break;
 case 5: Espiral(); break;
 default:circunferencia();break;
 }

 glFlush();

}

// Se ejecuta cuando se pulsa una tecla
void Opciones(unsigned char key, int x, int y){

 switch (key) {
 case 'a': opcion = 1; break;
 case 's': opcion = 2; break;
 case 'd': opcion = 3; break;
 case 'f': opcion = 4; break;
 case 'g': opcion = 5; break;
 default:  opcion = 0; break;
 }
 DibujaEscena();

}

int main(){
 // Funciones GLUT para inicializar la ventana
 glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
 glutInitWindowSize (500, 500);
 glutCreateWindow ("Practica 1 Eduardo Mora");
 // Indicamos la funcion que debe ejecutarse si se pulsa una tecla
 glutKeyboardFunc(Opciones);
 // Indicamos la función para el evento ‘Paint’
 glutDisplayFunc (DibujaEscena);
 // Indicamos la función para el evento ‘Resize’
 glutReshapeFunc (EscalaVentana);
 // Lanzamos el bucle indefinido de eventos
 glutMainLoop();
}
