#include <stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#define GL_PI 3.1416f

/*
    Macro para dibujar un cilindro, dado que no existe la funci?n glutSolidCylinder(), la creamos utilizando gluCylinder, m?s gen?rica.
*/
#define glutSolidCylinder(radio, altura, slices, stacks) gluCylinder(gluNewQuadric(), radio, radio, altura, slices, stacks)

/*
    Inicializacion de variables para iluminacion
*/
GLfloat mat_diffuse [] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_specular [] = {0.5, 0.5, 0.5, 1.0};
GLfloat mat_shininess [] = {50};
GLfloat mat_emission [] = {0.5, 0.2, 0.2, 1.0};

GLfloat light_diffuse [] = {1.0, 1.0, 0.0, 0.0};
GLfloat light_specular [] = {0.5, 0.5, 0.5, 1.0};
GLfloat light_position [] = {-7.0, 12.0, 0.0, 1.0};
GLfloat light_ambient [] = {0.5, 0.5, 0.5, 1.0};

GLfloat light_position_Foco [] = {0.0, 2.8, -0.15, 1.0};
GLfloat direccion[] = {-1.0, 1.0, 1.0};

/*
    Variables Globales
*/
GLdouble angulo = GL_PI/2, incremento_angulo=0.01;
GLdouble angulof=0.01, incremento_angulof=0.01;
GLfloat  radio=7.5;

GLfloat px0=0,py0=0,pz0=0; // posicion del ojo
GLfloat px1=0,py1=0,pz1=0; // punto de vista-centro

static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 1 ;
GLint parar = 1;
GLint encender = 0;

void display (void);

/*
    Iluminacion
*/
void iluminacionAmbiente (){

  glLightfv(GL_LIGHT0,GL_POSITION, light_position);
  glLightfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glLightfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glLightfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
}

void iluminacionFoco (){

  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position_Foco);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direccion);


  if(encender == 1){
    glEnable (GL_LIGHT1);
  }else {
    glDisable (GL_LIGHT1);
  }

}

/*
    Menu del raton
*/
void menu1(int i){
  switch(i){
    case 1:  encender = 1; break;
    case 2:  encender = 0; break;
    case 0 : exit(0); break;
  }
  display();
}

/*
    Inicio de la aplicacion
*/
void iniciar(void) {

  //Definimos el fondo
  glClearColor (1.0, 1.0, 1.0, 0.0);

  //Definimos sombreado suave
  glShadeModel(GL_SMOOTH);

  //Opciones del Menu
  glutCreateMenu(menu1);
  glutAddMenuEntry("encender lampara",1);
  glutAddMenuEntry("apagar lampara",2);
  glutAddMenuEntry("salir",0);

  //Definimos el boton derecho del mouse
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  // Especifica la combinacion de colores
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // Especifica el algoritmo para realizar el suavizado
  glHint(GL_LINE_SMOOTH_HINT,GL_DONT_CARE);

  //Muestra por consola las distintas opciones de teclado
  printf("Las funciones de teclado\n");
  printf("La tecla '+' avanza y '-' aleja la camara\n");
  printf("La teclas 'a' 'd' 's' 'w' mueven la camara\n");
  printf("La tecla 'v' sube el punto de mira\n");
  printf("La tecla 'b' baja el punto de mira\n");
  printf("La tecla 'y' activa/desactiva rotacion sobre el eje y\n");
  printf("La tecla 'l' enciende/apaga la lampara\n");
  printf("La teclas 'f' finaliza el programa\n");
}

/*
    Funcion para rotar la camara
*/
void rotacamara(){

  // El angulof debe variar entre 2*GL_PI y -2*GL_PI
  if (angulof > 2*GL_PI || angulof < -2*GL_PI )  angulof = 0.01;

  // El angulof debe variar entre 0 y GL_PI, hay que evitar que tenga valores negativos
  if (angulo > GL_PI - 0.01 )  angulo = GL_PI - 0.01;

  if (angulo < 0.02)  angulo =  0.01;

  //Calculamos los puntos
  px0=radio*sin(angulo)*cos(angulof);
  pz0=radio*sin(angulo)*sin(angulof);
  py0=radio*cos(angulo);

  //Mostramos por consola los puntos
  printf("posicion del ojo x: %f y: %f z: %f  \n angulo: %f angulof: %f \n",px0,py0,pz0, angulo, angulof);
}


/*
    Display de la imagen
*/
void display(void) {

  //color del fondo
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Se activa el suavizado de lineas
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);

  //La camara se desplaza sobre el plano x z con la y fija
  glPushMatrix();
  glLoadIdentity();
  rotacamara();
  gluLookAt(px0,py0,pz0,px1,py1,pz1,0,1,0);

  //Iluminación la figura
  iluminacionAmbiente();
  iluminacionFoco ();

  //Creamos el tablero
  glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.0f);//yellow
    glBegin(GL_QUAD_STRIP );
        glVertex3f(-3.0f, 0.0f, -3.0f);
        glVertex3f(-3.0f, 0.0f, 3.0f);
        glVertex3f(3.0f,  0.0f, -3.0f);
        glVertex3f(3.0f,  0.0f, 3.0f);
    glEnd();
  glPopMatrix();

  //Creamos la bola auxiliar 1
  glPushMatrix();
    glColor3f(0.5f, 1.0f, 1.0f);//cyan
    glTranslatef(-2.0f, 1.0f, -2.0f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  //Rotamos la lampara y las esferas si es necesario
  glRotatef(theta[0],1.0,0.0,0.0);
  glRotatef(theta[1],0.0,1.0,0.0);
  glRotatef(theta[2],0.0,0.0,1.0);

  //Creamos la Superficie de la lampara
  glPushMatrix();
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glColor3f(0.0f, 0.0f, 1.0f);//Blue
    glutSolidCone(1.0f,0.75f,10,10);
  glPopMatrix();

  //Creamos la primera bola de la base de la lampara
  glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.0f);//Bronze
    glTranslatef(0.0f, 0.37f, -0.75f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  //Creamos el Palo inferior de la lampara
  glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);//gris
    glTranslatef(0.0f, 0.3f, -0.75f);
    glRotatef(135.0, -1.0, 0.0, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos la segunda bola de la lamapara
  glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.0f);//Bronze
    glTranslatef(0.0f, 1.7f, -2.1f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  //Creamos el Palo superior de la lampara
  glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);//gris
    glTranslatef(0.0f, 3.2f, -0.75f);
    glRotatef(-135.0, -1.0, 0.0, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos la bola superior de la lamapara
  glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.0f);//Bronze
    glTranslatef(0.0f,3.2f, -0.75f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  //Creamos el foco de la lampara
  glPushMatrix();
    glTranslatef(0.0f, 2.8f, -0.15f);
    glRotatef(180.0, 0.0, -1.0, 0.0);
    glRotatef(45.0, -1.0, 0.0, 0.0);
    glColor3f(0.0f, 0.0f, 1.0f);//Blue
    glutSolidCone(0.75f,0.75f,10,10);
  glPopMatrix();

  //Creamos la bola auxiliar 2
  glPushMatrix();
   glColor3f(0.5f, 1.0f, 1.0f);//cyan
   glTranslatef(0.0f, 1.0f, 1.0f);
   glutSolidSphere(0.5,160,160); // Bola del Chupa-Chus
  glPopMatrix();

  //Terminamos de dibujar
  glFlush();
  glPopMatrix();
  glutSwapBuffers();
}

/*
    Animar la figura
*/
void Animar(){
  if (parar == 0){
      theta[axis] += .5;
      if(theta[axis]>360) theta[axis] -= 360.0;
      display();
  }
}

/*
    Opciones del teclado
*/
void teclado(unsigned char tecla,int x,int y) {

  switch(tecla){
    case '+' : radio = radio - 0.1;
               printf("posicion del ojo x: %f y: %f z: %f \n",px0,py0,pz0);
               break;
    case '-' : radio = radio + 0.1;
               printf("posicion del ojo x: %f y: %f z: %f \n",px0,py0,pz0);
               break;
    case 'w' : angulo=angulo-incremento_angulo;//rota camara  para arriba
               break;
    case 's' : angulo=angulo+incremento_angulo;//rota camara  para abajo
               break;
    case 'a' : angulof=angulof+incremento_angulof;//rota camara  a la izquierda
               break;
    case 'd' : angulof=angulof-incremento_angulof;//rota camara  a la derecha
               break;
    case 'v' : py1 = py1 + 0.1; //sube punto de mira
               break;
    case 'b' : py1 = py1 - 0.1; //baja punto de mira
               break;
    case 'l' : if (encender == 0) encender = 1; else encender = 0; break;//Enciende/Apaga el foco
               break;

    case 'y' : if (parar == 0) parar = 1; else parar = 0; break; //Anima la figura

    case 'f' : exit(0); break; //Finaliza la app
    }
   display();
}

/*
    Reescalado de la pantalla
*/
void myReshape(int w, int h) {
     glViewport (0, 0, (GLsizei) w, (GLsizei) h);
     glMatrixMode (GL_PROJECTION);
     glLoadIdentity ();
     gluPerspective(60.0, (GLfloat) w/(GLfloat) h,0.1, 30.0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
}

/*
    Main del sistema
*/
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("PEC3-EDUARDO");
    iniciar();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(Animar);
    glutKeyboardFunc(teclado);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
