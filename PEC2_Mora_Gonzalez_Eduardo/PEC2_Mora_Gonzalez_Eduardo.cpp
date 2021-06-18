#include <windows.h>
#include <GL/glut.h>

//ANGULOS GIRO EJES
GLfloat anguloX=0.0;
GLfloat anguloY=0.0;
GLfloat anguloZ=0.0;

//VALOR DE ESCALADO
GLfloat escalado=1.0;

//ANGULO GIRO TETERA
GLfloat anguloT=0.0;

void  funcionDePintado (void){

//COLOR DE FONDO
glClearColor(0.6, 0.6, 0.6, 1.0); //GRIS
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glLoadIdentity();


//POSICION PUNTO DE VISTA
glTranslatef(-1.0, -1.0, -13.0);

//ESCALADO
glScaled(escalado, escalado, escalado);

//ROTACION
glRotatef(anguloX, 1.0, 0.0, 0.0); //EJE X
glRotatef(anguloY, 0.0, 1.0, 0.0); //EJE Y
glRotatef(anguloZ, 0.0, 0.0, 1.0); //EJE Z

glColor3f (0.0, 1.0, 0.0);

//EJES

//EJES POSITIVOS

//GROSOR DE LAS LINEAS
glLineWidth(3);

glBegin (GL_LINES);
  glColor3f (0.0, 0.0, 0.0); // DEFINIMOS EL COLOR NEGRO
  //EJE X
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(10.0,0.0,0.0);
  //EJE Y
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,10.0,0.0);
  //EJE Z
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,10.0);
glEnd();

//EJESO NEGATIVOS

//GROSOR DE LAS LINEAS
glLineWidth(1);

glBegin (GL_LINES);
  //EJE X
  glColor3f (1.0, 0.0, 0.0); // ROJO
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(-10.0,0.0,0.0);
  //EJE Y
  glColor3f (0.0, 0.0, 1.0); // AZUL
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,-10.0,0.0);
  //EJE Z
  glColor3f (1.0, 1.0, 0.0); // AMARILLO
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,-10.0);
glEnd();

//CONOS

//EJE X
glPushMatrix();
    glColor3f (1.0, 0.0, 0.0); // ROJO
    glTranslated(9.5,0.0,0.0);
    glRotated(90.0,0.0,1.0,0.0); //Rotamos la figura
    glutWireCone(0.2,1, 15, 15);
glPopMatrix();
//EJE Y
glPushMatrix();
    glColor3f (0.0, 0.0, 1.0); // AZUL
    glTranslated(0.0,9.5,0.0);
    glRotated(270.0,1.0,0.0,0.0); //Rotamos la figura
    glutWireCone(0.2,1, 15, 15);
glPopMatrix();
//EJE Z
glPushMatrix();
    glColor3f (1.0, 1.0, 0.0); // AMARILLO
    glTranslated(0.0,0.0,9.5);
    glutWireCone(0.2,1, 15, 15);
glPopMatrix();


//CUBO PRINCIPAL
glColor3f( 0.0, 1.0, 0.0 ); glVertex3f( 0.5, 0.5, -0.5 );  // VERDE
glutWireCube(10.0);

//FIGURAS Y NEGATIVA

//TETAEDRO
glPushMatrix();
glTranslated(5.0,-5.0,5.0);
glutWireTetrahedron();
glPopMatrix();

//DODECAEDRO
glPushMatrix();
glTranslated(-5.0,-5.0,5.0);
glScalef (0.75f, 0.75f, 0.75f); //ESCALAMOS LA FIGURA
glutWireDodecahedron();
glPopMatrix();

//CUBO
glPushMatrix();
glTranslated(5.0,-5.0,-5.0);
glutWireCube(1.5);
glPopMatrix();

//TETERA
glPushMatrix();
glColor3f (1.0, 0.0, 1.0);//MAGENTA
glTranslated(-5.0,-5.0,-5.0);

//ROTACION
glRotatef(anguloX, 1.0, 0.0, 0.0); //EJE X MANTENGO EL ACTUAL
glRotatef(anguloT, 0.0, 1.0, 0.0); //EJE Y DONDE HAGO LA ROTACION
glRotatef(anguloZ, 0.0, 0.0, 1.0); //EJE Z MANTENGO EL ACTUAL

glutWireTeapot(1.0);
glPopMatrix();

//FIGURAS Y POSTIVA

//ESFERA
glPushMatrix();
glColor3f( 0.0, 1.0, 0.0 ); glVertex3f( 0.5, 0.5, -0.5 );//VERDE
glTranslated(5.0,5.0,5.0);
glutWireSphere(1.0,25,25);
glPopMatrix();

//TORO
glPushMatrix();
glTranslated(-5.0,5.0,5.0);
glutWireTorus(0.5,1.0,50,20);
glPopMatrix();

//CILINDRO 1
glPushMatrix();
glTranslated(5.0,5.0,-5.0);
glRotated(90.0,1.0,0.0,0.0); //ROTAMOS LA FIGURA
GLUquadricObj * qobj1 = gluNewQuadric(); //CREAR OBJETO CUADRETICO
gluQuadricDrawStyle(qobj1, GLU_LINE); //ALÁMBRICO
gluCylinder(qobj1, 1.0, 1.0, 1.0, 30, 30); //DIBUJAR CILINDRO
glPopMatrix();

//CILINDRO 2
glPushMatrix();
glTranslated(-5.0,5.0,-5.0);
GLUquadricObj * qobj2 = gluNewQuadric(); //CREAR OBJETO CUADRETICO
gluQuadricDrawStyle(qobj2, GLU_LINE); //ALÁMBRICO
gluCylinder(qobj2, 1.0, 1.0, 1.0, 30, 30); //DIBUJAR CILINDRO
glPopMatrix();

glFlush();
}


void  funcionDeReescalado(GLsizei w, GLsizei h){

glMatrixMode (GL_PROJECTION); //ACTIVAR MATRIZ PROYECCION
glLoadIdentity ();

gluPerspective(100.0, (float)w/(float)h,1.0, 150.0); //PESPECTIVA

//DEFINICION VOLUMEN VISUALIZACION
glViewport (0, 0, w, h); //DEFINIR VIEWPORT
glMatrixMode (GL_MODELVIEW);

}

void funcionRotar(unsigned char key, int x, int y){

  switch(key){
   case 'x':
        anguloX+=0.5;
        if(anguloX>=360) anguloX=0.0;
        funcionDePintado();
        break;
   case 'X':
        anguloX-=0.5;
        if(anguloX <=0) anguloX=360.0;
        funcionDePintado();
        break;
   case 'y':
        anguloY+=0.5;
        if(anguloY>=360) anguloY=0.0;
        funcionDePintado();
        break;
   case 'Y':
        anguloY-=0.5;
        if(anguloY <=0) anguloY=360.0;
        funcionDePintado();
        break;
   case 'z':
        anguloZ+=0.5;
        if(anguloZ>=360) anguloZ=0.0;
        funcionDePintado();
        break;
   case 'Z':
        anguloZ-=0.5;
        if(anguloZ <=0) anguloZ=360.0;
        funcionDePintado();
        break;
   case 'e':
        escalado -= 0.0255;

        funcionDePintado();
        break;
   case 'E':
        escalado += 0.0255;
        funcionDePintado();
        break;
   case 't':
        glPushMatrix();
        anguloT+=2.0;
        if(anguloT>=360) anguloT=0.0;
        funcionDePintado();
        break;
  }
}



int main(int argc, char** argv){

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(0,0);
  glutCreateWindow("PC02_EDUARDO_MORA");
  glPointSize(6.0);

  glEnable(GL_DEPTH_TEST);

  glutReshapeFunc(funcionDeReescalado);
  glutKeyboardFunc(funcionRotar);
  glutDisplayFunc(funcionDePintado);
  glutMainLoop();

  return 0;
}
