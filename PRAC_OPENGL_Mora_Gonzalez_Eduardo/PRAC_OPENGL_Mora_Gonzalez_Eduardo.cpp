#include <windows.h>
#include <FreeImage.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <gl/glut.h>

/*******************************************************************************
*                             VARIABLES GLOBALES                               *
*******************************************************************************/

#define DEB 0
#define PI  3.141592
#define GL_PI 3.1416f
#define glutSolidCylinder(radio, altura, slices, stacks) gluCylinder(gluNewQuadric(), radio, radio, altura, slices, stacks)

void displayevent(void);
void menu1(int);
void reshapeevent(GLsizei width, GLsizei height);

/**
* Tema de audio.
*/
#define TEMA "lavidaebella.wav"

/**
* Aumento/disminución de distancia al acercarse/alejarse, mirar arriba/abajo.
*/
#define MOV_DIST 0.05

/**
* Ángulo de giro horizontal (cámara).
*/
GLfloat anguloH = 0.8;

/**
* Ángulo de giro vertical (cámara).
*/
GLfloat anguloV = 0.96;

/**
* Distancia entre cámara y centro.
*/
GLfloat radio = 30;

/**
* Coordenada X del ratón.
*/
GLsizei rx = 0;

/**
* Coordenada Y del ratón.
*/
GLsizei ry = 0;

/**
* Movimiento rotacional de la cámara (en radianes).
*/
#define MOV_ROT 0.02

/**
* La música está sonando.
*/
GLboolean sonidoON = FALSE;

/**
* Para cambio de textura de suelo.
*/
GLint ts = 3;

/**
* Mostrar puntos de control.
*/
GLboolean puntosCtlON = FALSE;

/**
* Mostrar soleado/atardecer.
*/
GLboolean soleadoON = TRUE;

/**
* Cambiar Lona.
*/
GLboolean lonaTipo = 14;

/**
* Cambiar Alfombra.
*/
GLboolean alfombraTipo = 0;

/**
* Encender Lampara
*/
GLint encender = 0;

/**
* Itelacion Raton
*/
GLfloat angrY=0,angr=0, delta;
int aw, ah;

/**
* Animación Lampara
**/
static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 1 ;
GLint parar    = 1;

/**
* Rotación
**/
GLdouble angulo  = GL_PI/2, incremento_angulo=0.01;
GLdouble angulof = 0.01, incremento_angulof=0.01;
GLfloat ox=15.0,oy=5.0,oz=15.0, ang, paso, dtx, dtz, py1 = 5.0;

/**
* Inicio
**/
GLboolean inicioDibujo = TRUE;

/*******************************************************************************
*                                ILUMINACION                                  *
*******************************************************************************/


/**
* Parámetros referentes a la luz ambiental y su incidencia sobre los materiales.
*/
GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_emission[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat mat_shininess[] = { 50 };

GLfloat light_amb_position[] = { 0, 5, 0, 1 };
GLfloat light_amb[] = { 1.0, 1.0, 1.0, 1.0 }; //soleado
GLfloat light_amb_diffuse[] = { 0, 0, 0, 1.00 };
GLfloat light_amb_specular[] = { 0, 0, 0, 1.00 };

GLfloat light_diffuse  [] = {1.0, 1.0, 0.0, 0.0};
GLfloat light_specular [] = {0.5, 0.5, 0.5, 1.0};
GLfloat light_position_Foco [] = {0.0, 4.4, 0.75, 1.0};
GLfloat direccion[] = {0.5, 3.0, 16.0};


/**
* Iluminar lampara de la mesa
*/
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

/**
* Tratamiento de luces.
*/
void encenderLuces() {
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}


/*******************************************************************************
*                                  TEXTURAS                                    *
*******************************************************************************/

struct TTexture {
	char * fileName;
	int  transparencia; // 1 si, 0: no
	unsigned char colortransparente[3];
};

TTexture texturefiles[] = {
	{ "alfombra1.bmp", 0,   { 0,0,0 } },     // 0
	{ "ceramica.bmp", 0,    { 0,0,0 } },     // 1
	{ "metal.bmp", 0,       { 0,0,0 } },     // 2
	{ "madera.bmp", 0,      { 0,0,0 } },     // 3
	{ "metal.bmp", 0,       { 0,0,0 } },     // 4
	{ "pared.bmp", 0,       { 0,0,0 } },     // 5
	{ "oxido.bmp", 0,       { 0,0,0 } },     // 6
	{ "cuadro.bmp", 0,      { 0,0,0 } },     // 7
    { "hierba.bmp", 0,      { 0,0,0 } },     // 8
	{ "cielo.bmp", 0,       { 0,0,0 } },     // 9
    { "tierra.bmp", 0,      { 0,0,0 } },     // 10
    { "tronco.bmp", 0,      { 0,0,0 } },     // 11
    { "hojas1.bmp", 0,      { 0,0,0 } },     // 12
	{ "hojas2.bmp", 0,      { 0,0,0 } },     // 13
	{ "toldo1.bmp", 0,      { 0,0,0 } },     // 14
    { "toldo2.bmp", 0,      { 0,0,0 } },     // 15
 	{ "alfombra2.bmp", 0,   { 0,0,0 } },     // 16
};

#define NTextures sizeof(texturefiles)/sizeof(TTexture)

GLuint	texture[NTextures];

void LoadTexture(int i){
char* strFileName =texturefiles[i].fileName;

	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(strFileName, 0); //PBL

	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(strFileName, 0),strFileName);
	FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);

	if (bitmap)
	{

		glGenTextures(1, &texture[i]);

		glBindTexture(GL_TEXTURE_2D, texture[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			nWidth,
			nHeight,
			0,
			32993,// valor de GL_BGRA // https://docs.factorcode.org/content/word-GL_BGRA%2Copengl.gl.html
			GL_UNSIGNED_BYTE,
			(void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
		FreeImage_Unload(bitmap);
	}

}

void LoadTextures(){
	for (int i=0; i<NTextures; i++ )
		LoadTexture(i);
}

void CargarTexturasGenerales(){
  GLfloat plano_s[4] = {1, 0, 0, 0}; // s=x
  GLfloat plano_t[4] = {0, 1, 0, 0}; // t=y
  glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv (GL_S, GL_OBJECT_PLANE, plano_s);
  glEnable (GL_TEXTURE_GEN_S);
  glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGenfv (GL_T, GL_OBJECT_PLANE, plano_t);
  glEnable (GL_TEXTURE_GEN_T);
}


/*******************************************************************************
*                                   BEZIER                                     *
*******************************************************************************/

GLUquadricObj *cilindro;

GLint numPtos;
GLint densidad;
GLfloat xMax, yMax;

GLint nNumPoints = 3;
GLfloat ctrlPoints[5][3][3]={
       {{ -4.0f,0.0f,4.0f},{-2.0f,0.0f,4.0f},{4.0f,0.0f,4.0f}}, // v=0  u=0,1,2
       {{-4.0f,0.0f,2.0f},{-2.0f,4.0f,2.0f},{4.0f,0.0f,2.0f}},  // v=1 u=0,1,2
       {{-4.0f,0.0f,0.0f},{-2.0f,8.0f,0.0f},{4.0f,0.0f,0.0f}},// v=2 u=0,1,2
       {{-4.0f,0.0f,-2.0f},{-2.0f,4.0f,-2.0f},{4.0f,0.0f,-2.0f}},// v=3 u=0,1,2
       {{-4.0f,0.0f,-4.0f},{-2.0f,0.0f,-4.0f},{4.0f,0.0f,-4.0f}}};// v=4 u=0,1,2

//coordenadas de la textura de Bezier
GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}},
              {{1.0, 0.0}, {1.0, 1.0}}};


/*******************************************************************************
*                                   SONIDO                                     *
*******************************************************************************/

 // Apaga / enciende el sonido.
void cambiarSonido() {
	sonidoON = !sonidoON;
	if (sonidoON) {
		PlaySound(TEMA, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}
	else {
		PlaySound(NULL, NULL, 0);
	}
}


/*******************************************************************************
*                                 MENÚ RATON                                   *
*******************************************************************************/

// Opciones del Menú de ratón de la aplicación.
void menu1(int i){
	switch (i){
	case 1:
		cambiarSonido();
		break;
	case 2:
        soleadoON = !soleadoON;
		if (soleadoON) {light_amb[1] =1.0; light_amb[2] =1.0;  }
         else { light_amb[1] =0.0; light_amb[2] =0.0;  };
        encenderLuces();
		break;
	case 3:
		if (lonaTipo == 14){ lonaTipo = 15;
		}else lonaTipo = 14;
		break;
	case 4:
		if (alfombraTipo == 0){ alfombraTipo = 16;
		}else alfombraTipo = 0;
		break;
	case 5:
        puntosCtlON = !puntosCtlON;
		break;
	case 6:
        encender = 1;
		break;
	case 7:
        encender = 0;
		break;
	case 0:
		exit(0);
		break;
	}
	displayevent();
}

/**
* Crea el menú contextual.
*/
void crearMenuContextual() {
	glutCreateMenu(menu1);
	glutAddMenuEntry("Música ON/OFF", 1);
	glutAddMenuEntry("Cambiar Soleado/Atardecer", 2);
	glutAddMenuEntry("Cambiar Lona", 3);
	glutAddMenuEntry("Cambiar Alfombra", 4);
    glutAddMenuEntry("Mostrar Puntos de Control", 5);
    glutAddMenuEntry("Encender Lampara", 6);
    glutAddMenuEntry("Apagar Lampara", 7);
	glutAddMenuEntry("Salir", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/*******************************************************************************
*                                MENÚ CONSOLA                                  *
*******************************************************************************/

/**
* Muestra las instrucciones del programa.
*/
void mostrarInstrucciones() {

	printf("  ____________________________________________ \n");
	printf(" |                                            |\n");
	printf(" |              EDUARDO MORA GONZALEZ         |\n");
	printf(" |____________________________________________|\n");
	printf(" |                                            |\n");
	printf(" |             PRACTICA FINAL OPENGL          |\n");
	printf(" |____________________________________________|\n");
	printf(" |                                            |\n");
	printf(" |            FUNCIONALIDADES TECLADO:        |\n");
	printf(" |                                            |\n");
	printf(" | <+> Zoom in                                |\n");
	printf(" | <-> Zoom out                               |\n");
	printf(" | <a> Mover Izquierda                        |\n");
	printf(" | <s> Mover Abajo                            |\n");
	printf(" | <d> Mover Derecha                          |\n");
	printf(" | <w> Mover Arriba                           |\n");
	printf(" | <v> Sube Punto de Mira                     |\n");
	printf(" | <b> Baja Punto de Mira                     |\n");
	printf(" | <y> Animacion Lampara                      |\n");
	printf(" | <l> Enciende/Apaga la Lampara              |\n");
	printf(" | <f> Fin del programa                       |\n");
	printf(" |                                            |\n");
	printf(" |____________________________________________|\n");
	printf(" |                                            |\n");
	printf(" | <Click raton> Desplazamiento de la camara  |\n");
	printf(" |____________________________________________|\n");
	printf(" |                                            |\n");
	printf(" |            FUNCIONALIDADES RATÓN:          |\n");
	printf(" |                                            |\n");
	printf(" | <1> Música ON/OFF                          |\n");
	printf(" | <2> Cambiar Soleado/Atardecer              |\n");
	printf(" | <3> Cambiar Lona                           |\n");
	printf(" | <4> Cambiar Alfombra                       |\n");
	printf(" | <5> Mostrar Puntos de Control              |\n");
	printf(" | <6> Encender Lampara                       |\n");
	printf(" | <7> Apagar Lampara                         |\n");
	printf(" | <0> Fin del programa                       |\n");
	printf(" |                                            |\n");
	printf(" |____________________________________________|\n");

}

/*******************************************************************************
*                             GIROS DE LA ESCENA                               *
*******************************************************************************/

/*
    Funcion para rotar la camara
*/
void rotacamara(){

  // El angulof debe variar entre 2*GL_PI y -2*GL_PI
  if (anguloH > 2*GL_PI || anguloH < -2*GL_PI )  anguloH = 0.01;

  // El angulof debe variar entre 0 y GL_PI, hay que evitar que tenga valores negativos
  if (anguloV > GL_PI - 0.01 )  anguloV = GL_PI - 0.01;

  if (anguloV < 0.02)  anguloV =  0.01;

  //Calculamos los puntos
  ox=radio*sin(anguloV)*cos(anguloH);
  oz=radio*sin(anguloV)*sin(anguloH);
  oy=radio*cos(anguloV);

}

/*******************************************************************************
*                             OBJETO CUADRATICO                                *
*******************************************************************************/

GLUquadricObj *pQObj;

void DefineQuadric(){
		pQObj = gluNewQuadric();
		gluQuadricDrawStyle(pQObj, GLU_FILL);
		gluQuadricNormals(pQObj, GLU_SMOOTH);
		gluQuadricTexture(pQObj, GL_TRUE);
}


/*******************************************************************************
*                           ELEMENTOS DE LA ESCENA                             *
*******************************************************************************/

/*
    Funcion para crear el suelo de tierra
*/
void crearSueloTierra(){

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[10]);

  glPushMatrix();
  glBegin(GL_QUAD_STRIP);
     glVertex3f(-15.0f, -1.0f, -10.0f);
     glVertex3f(-15.0f, -1.0f, 10.0f);
     glVertex3f(15.0f,  -1.0f, -10.0f);
     glVertex3f(15.0f,  -1.0f, 10.0f);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}


/*
    Funcion para crear el soporte del suelo
*/
void crearSoportes(){

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[2]);

  //Creamos el soporte 1
  glPushMatrix();
    glTranslatef(-5.5f, -1.0f, -5.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(0.5f,1.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

    //Creamos el soporte 2
  glPushMatrix();
    glTranslatef(-5.5f, -1.0f, 5.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(0.5f,1.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

    //Creamos el soporte 3
  glPushMatrix();
    glTranslatef(5.5f, -1.0f, -5.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(0.5f,1.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

    //Creamos el soporte 4
  glPushMatrix();
    glTranslatef(5.5f, -1.0f, 5.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(0.5f,1.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}

/*
    Funcion para crear el suelo
*/
void crearSuelo(){

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[1]);

  glPushMatrix();
  glBegin(GL_QUAD_STRIP);
     glVertex3f(-6.0f, 0.01f, -6.0f);
     glVertex3f(-6.0f, 0.01f, 6.0f);
     glVertex3f(6.0f,  0.01f, -6.0f);
     glVertex3f(6.0f,  0.01f, 6.0f);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

/*
    Funcion para crear la alfombra
*/
void crearAlfombra(){

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[alfombraTipo]);

  //Creamos la alfombra
  glPushMatrix();
    glBegin(GL_QUAD_STRIP);
        glVertex3f(-5.0f, 0.012f, -5.0f);
        glVertex3f(-5.0f, 0.012f, 5.0f);
        glVertex3f(5.0f,  0.012f, -5.0f);
        glVertex3f(5.0f,  0.012f, 5.0f);
    glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}

/*
    Funcion para crear las paredes
*/
void crearParedes(){

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture[5] );

  //Creamos la pared del fondo
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
    glVertex3f(-6.0f,0.0f, -6.0f);
    glVertex3f(6.0f, 0.0f, -6.0f);
    glVertex3f(-6.0f,6.0f, -6.0f);
    glVertex3f(6.0f, 6.0f, -6.0f);
  glEnd();
  glPopMatrix();

  //Creamos la pared lateral 1
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
    glVertex3f(-6.0f, 0.0f, -6.0f);
    glVertex3f(-6.0f, 0.0f, 6.0f);
    glVertex3f(-6.0f, 6.0f, -6.0f);
    glVertex3f(-6.0f, 6.0f, 6.0f);
  glEnd();
  glPopMatrix();

  //Creamos la pared lateral 2
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
    glVertex3f(6.0f,0.0f, -6.0f);
    glVertex3f(6.0f, 0.0f, 6.0f);
    glVertex3f(6.0f,6.0f, -6.0f);
    glVertex3f(6.0f, 6.0f, 6.0f);
  glEnd();
  glPopMatrix();
  glDisable( GL_TEXTURE_2D );
}

/*
    Funcion para crear la mesa
*/
void crearMesa(){

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture[3] );

  //Creamos la tabla superior de la mesa
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
      glVertex3f(-3.5f, 2.0f, -3.5f);
      glVertex3f(-3.5f, 2.0f, 3.5f);
      glVertex3f(3.5f,  2.0f, -3.5f);
      glVertex3f(3.5f,  2.0f, 3.5f);
  glEnd();
  glPopMatrix();

  //Creamos la tabla inferior de la mesa
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
     glVertex3f(-3.5f, 1.8f, -3.5f);
     glVertex3f(-3.5f, 1.8f, 3.5f);
     glVertex3f(3.5f,  1.8f, -3.5f);
     glVertex3f(3.5f,  1.8f, 3.5f);
  glEnd();
  glPopMatrix();

  //Creamos lado 1 de la mesa
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
     glVertex3f(-3.5f, 1.8f, -3.5f);
     glVertex3f(-3.5f, 1.8f, 3.5f);
     glVertex3f(-3.5f, 2.0f, -3.5f);
     glVertex3f(-3.5f, 2.0f, 3.5f);
  glEnd();
  glPopMatrix();

  //Creamos lado 2 de la mesa
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
     glVertex3f(-3.5f, 1.8f, -3.5f);
     glVertex3f(3.5f,  1.8f, -3.5f);
     glVertex3f(-3.5f, 2.0f, -3.5f);
     glVertex3f(3.5f,  2.0f, -3.5f);
  glEnd();
  glPopMatrix();

  //Creamos lado 3 de la mesa
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
     glVertex3f(3.5f,  1.8f, -3.5);
     glVertex3f(3.5f,  1.8f, 3.5f);
     glVertex3f(3.5f,  2.0f, -3.5f);
     glVertex3f(3.5f,  2.0f, 3.5f);
  glEnd();
  glPopMatrix();

  //Creamos lado 4 de la mesa
  glPushMatrix();
  glBegin(GL_QUAD_STRIP );
    glVertex3f(-3.5f, 1.8f, 3.5);
    glVertex3f(3.5f,  1.8f, 3.5f);
    glVertex3f(-3.5f, 2.0f, 3.5);
    glVertex3f(3.5f,  2.0f, 3.5f);
  glEnd();
  glPopMatrix();

  //Creamos la pata 1 de la mesa
  glPushMatrix();
    glTranslatef(-3.25f, 2.0f, -3.25f);
    glRotatef(-90.0, -1.0, 0.0, 0.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos la pata 2 de la mesa
  glPushMatrix();
    glTranslatef(3.25f, 2.0f, -3.25f);
    glRotatef(-90.0, -1.0, 0.0, 0.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos la pata 3 de la mesa
  glPushMatrix();
    glTranslatef(-3.25f, 2.0f, 3.25f);
    glRotatef(-90.0, -1.0, 0.0, 0.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos la pata 4 de la mesa
  glPushMatrix();
    glTranslatef(3.25f, 2.0f, 3.25f);
    glRotatef(-90.0, -1.0, 0.0, 0.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  glDisable( GL_TEXTURE_2D );
}

/*
    Funcion para crear el cuadro
*/
void crearCuadro(){

  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture[7] );

  glPushMatrix();

  glBegin(GL_QUAD_STRIP );
     glVertex3f(-4.0f,2.0f, -5.98f);
     glVertex3f(4.0f, 2.0f, -5.98f);
     glVertex3f(-4.0f,4.0f, -5.98f);
     glVertex3f(4.0f, 4.0f, -5.98f);
  glEnd();
  glPopMatrix();

  glDisable( GL_TEXTURE_2D );

}

/*
    Funcion para dibujar  y rotar la lampara
*/
void crearLampara(){

  //Rotamos la lampara
  glRotatef(theta[0],1.0,0.0,0.0);
  glRotatef(theta[1],0.0,1.0,0.0);
  glRotatef(theta[2],0.0,0.0,1.0);

  //Cargamos Textura 1
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture[6] );

  //Creamos la primera bola de la base de la lampara
  glPushMatrix();
    glTranslatef(0.0f, 2.37f, -0.75f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  //Creamos la segunda bola de la lamapara
  glPushMatrix();
    glTranslatef(0.0f, 3.7f, -2.1f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  //Creamos la bola superior de la lamapara
  glPushMatrix();
    glTranslatef(0.0f,5.2f, -0.75f);
    glutSolidSphere(0.3,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  glDisable( GL_TEXTURE_2D );


  //Cargamos Textura 2
  glEnable( GL_TEXTURE_2D );
  glBindTexture( GL_TEXTURE_2D, texture[2] );

  //Creamos la Superficie de la lampara
  glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glutSolidCone(1.0f,0.75f,10,10);
  glPopMatrix();

  //Creamos el Palo inferior de la lampara
  glPushMatrix();
    glTranslatef(0.0f, 2.3f, -0.75f);
    glRotatef(135.0, -1.0, 0.0, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos el Palo superior de la lampara
  glPushMatrix();
    glTranslatef(0.0f, 5.2f, -0.75f);
    glRotatef(-135.0, -1.0, 0.0, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glutSolidCylinder(0.1f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  //Creamos el foco de la lampara
  glPushMatrix();
    glTranslatef(0.0f, 4.8f, -0.15f);
    glRotatef(180.0, 0.0, -1.0, 0.0);
    glRotatef(45.0, -1.0, 0.0, 0.0);
    glutSolidCone(0.75f,0.75f,10,10);
  glPopMatrix();

  glDisable( GL_TEXTURE_2D );
}

/*
    Funcion para dibujar  los arboles
*/
void crearArboles(){


  //Creamos los troncos de los arboles

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[11]);

  //Creamos el tronco del arbol grande 1
  glPushMatrix();
    glTranslatef(-13.0f, -1.0f, -12.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(1.0f,7.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

    //Creamos el tronco pequeño 1
  glPushMatrix();
    glTranslatef(-13.0f, -1.0f, 12.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(1.0f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

    //Creamos el tronco del arbol grande 2
  glPushMatrix();
    glTranslatef(13.0f, -1.0f, -12.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(1.0f,7.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

    //Creamos el tronco pequeño 2
  glPushMatrix();
    glTranslatef(13.0f, -1.0f, 12.5f);
    glRotatef(90,-1,0,0);
    glutSolidCylinder(1.0f,2.0f,20,1);  // Palo del Chupa-Chus
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

  //Creamos los arboles 1

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[12]);

  glPushMatrix();
    glTranslatef(13.0f, 8.5f, -12.5f);
    glutSolidSphere(4.5,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  glPushMatrix();
    glTranslatef(-13.0f, 8.5f, -12.5f);
    glutSolidSphere(4.5,160,160); // Bola del Chupa-Chus
   glPopMatrix();

  glDisable(GL_TEXTURE_2D);


  //Creamos los arboles 2

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[13]);

  glPushMatrix();
    glTranslatef(-13.0f, 1.0f, 12.5f);
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glutSolidCone(2.5f,7.75f,10,10);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(13.0f, 1.0, 12.5f);
    glRotatef(90.0, -1.0, 0.0, 0.0);
    glutSolidCone(2.5f,7.75f,10,10);
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

}

/*
    Funcion para crear la lona
*/
void crearLona(){

     int i,j;

  glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, texture[lonaTipo]);

  glPushMatrix();
    glTranslatef(0.0f, 6.0, 0.0f);
    glScalef(1.5f, 1.5f, 1.5f);
    glRotatef(270,0,1,0);
    // Sets up the bezier
	// This actually only needs to be called once and could go in
	// the setup function
	glMap2f(GL_MAP2_VERTEX_3,	// Type of data generated
	0.0f,						// Lower u range
	20.0f,						// Upper u range
	3,							// Distance between points in the data
	3,							// Dimension in u direction (order)
	0.0f,						// Lover v range
	10.0f,						// Upper v range
	9, //15 9							// Distance between points in the data
	5, //3 5							// Dimension in v direction (order)
	&ctrlPoints[0][0][0]);		// array of control points

	// Enable the evaluator
	glEnable(GL_MAP2_VERTEX_3);

	// Use higher level functions to map to a grid, then evaluate the
	// entire thing.

	// Map a grid of 10 points from 0 to 10
	glMapGrid2f(20,0.0f,20.0f,10,0.0f,10.0f);

	// Evaluate the grid, using superficies
	glEvalMesh2(GL_FILL,0,20,0,10);

   /* El siguiente codigo dibuja los puntos de control */
    if (puntosCtlON){
        glDisable(GL_TEXTURE_2D);
        glPointSize(8.0);
        glColor3f(0.0f, 0.0f, 0.0f);//Black
        glBegin(GL_POINTS);
            for (i = 0; i < 5; i++){
              for (j = 0; j < nNumPoints; j++)
                glVertex3fv(&ctrlPoints[i][j][0]);
            }
        glEnd();
    }

  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}

/*
    Funcion para crear El Escenario Principal
*/
void crearEscenario(){

	// dibuja el cielo (una esfera)
    glColor3f(1,1,1);
	glTranslatef(11,-5,-11);
	glRotatef(90,0,0,1);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(1.0f, 1.0f, 1.0f);
    gluSphere(pQObj, 48, 10, 10);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Piso circular
	glPushMatrix();
	glColor3f(0.6,0.6,0.6);
	glEnable( GL_TEXTURE_2D );
    glBindTexture(GL_TEXTURE_2D, texture[8]);
	glTranslatef(0,-1.02,0);
	glRotatef(90,1,0,0);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(20.0f, 20.0f, 1.0f);
    gluDisk(pQObj,0.01, 43 , 20, 20 );
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
    glDisable( GL_TEXTURE_2D );
}


/*******************************************************************************
*                               DIBUJAR ESCENA                                 *
*******************************************************************************/

void displayevent(void){

  //limpia la escena (ventana)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Se activa el suavizado de lineas
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);

  glPushMatrix();
  glLoadIdentity();

  //Iluminación la figura
  encenderLuces();
  iluminacionFoco();

  crearEscenario();

  CargarTexturasGenerales();

  //Creamos los elementos de la escena
  crearSueloTierra();
  crearSoportes();
  crearSuelo();
  crearAlfombra();
  crearParedes();
  crearCuadro();
  crearMesa();
  crearArboles();
  crearLona();
  crearLampara();

  //Desabilitamos las texturas generales
  glDisable (GL_TEXTURE_GEN_S);
  glDisable (GL_TEXTURE_GEN_T);

  //Terminamos de dibujar
  glFlush();
  glPopMatrix();
  glutSwapBuffers();
}

/*******************************************************************************
*                                  INTERACCION                                 *
*******************************************************************************/

/**
    Animar la figura
**/
void Animar(){
  if (parar == 0){
      theta[axis] += .5;
      if(theta[axis]>360) theta[axis] -= 360.0;
      displayevent();
  }
}

/**
* Función captura de teclas.
*/
void leerTeclado(unsigned char key, int a, int b) {

	//GLfloat oxAux = ox, oyAux = oy, ozAux = oz, radioAux = radio, anguloHAux = anguloH;


	switch (key) {

	case '+': //Zoom IN
		radio = radio - 0.1;

		break;
	case '-': //Zoom OUT
		radio = radio + 0.1;

		break;

    case 'w' : anguloV=anguloV-incremento_angulo;//rota camara  para arriba

		break;
    case 's' : anguloV=anguloV+incremento_angulo;//rota camara  para abajo

		break;
    case 'a' : anguloH=anguloH+incremento_angulof;//rota camara  a la izquierda

		break;
    case 'd' : anguloH=anguloH-incremento_angulof;//rota camara  a la derecha

		break;
    case 'v' : py1 = py1 + 0.2; //sube punto de mira
               break;
    case 'b' : py1 = py1 - 0.2; //baja punto de mira

               break;
    case 'l' : if (encender == 0) encender = 1; else encender = 0; break;//Enciende/Apaga el foco
               break;

    case 'y' : if (parar == 0) parar = 1; else parar = 0; break; //Anima la figura

    case 'f' : exit(0); break; //Finaliza la app
	}


	displayevent();
}

/**
* Función de lectura de movimiento de ratón.
*/
void leerMov(GLsizei x, GLsizei y) {
	GLfloat oyAux = oy, anguloHAux = anguloH;
	if (abs(rx - x) > 1) {
		if (x > rx) {
			anguloH -= MOV_ROT;
		}
		else {
			anguloH += MOV_ROT;
		}
		ox = radio * sin(anguloH);
		oz = radio * cos(anguloH);
	}
	if (abs(ry - y) > 1){
		if (y > ry) {
			anguloV += MOV_ROT;
		}
		else {
			anguloV -= MOV_ROT;
		}
		oy = radio * sin(anguloV);
	}
	rx = x;
	ry = y;
	if (anguloH < -0.08 || anguloH > 1.44)  {
		oy = oyAux;
		anguloH = anguloHAux;
	}
}

/*******************************************************************************
*                              METODOS PRINCIPALES                             *
*******************************************************************************/

void idleevent(){
	// mueve la camara
	reshapeevent(aw,ah);

	Animar();
	// redibuja la escena
	glutPostRedisplay();
}

void reshapeevent(GLsizei width, GLsizei height){

    aw=width;
    ah=height;
	// establecer el área de visualizacion en la ventana
    glViewport(0,0,width,height);

	// seleccionar la matriz de Proyección
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Vista en Perspectiva
    gluPerspective(70,(GLfloat)width/(GLfloat)height,  0.028, 100);

    //Camara y sus posiciones
    rotacamara();

    gluLookAt( ox, oy, oz,  10, py1, 0,  0,1,0);

	// Restaurar a la matriz del Modelo (escena)
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

}

/**
* Función llamada al inicio una única vez.
*/
void init(void) {
	glEnable(GL_DEPTH_TEST);		//Se activa la variable para que funcione el z-Buffer
	glMatrixMode(GL_MODELVIEW);		//Activar matriz de modelo-vista como activa

	glColor3f(1, 1, 1);

	mostrarInstrucciones();

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_amb_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_amb_specular);

	crearMenuContextual();
}

int main(int argc, char** argv){

	// inicialización del GLUT
	glutInit( &argc, argv );

	// inicialiación de la ventana
	glutInitWindowSize( 300, 300 );
	glutInitWindowPosition( 100, 100 );
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA);
    glutCreateWindow( "" );
	glEnable(GL_DEPTH_TEST);
	glutSetWindowTitle( "EDUARDO MORA GONZALEZ" );
	init();									//Inicio
    // para luces
  	glEnable(GL_LIGHTING);					//Activar las luces
	glEnable(GL_COLOR_MATERIAL);			//Dejar que se vea el color

	// Carga las Texturas
	glEnable( GL_DEPTH_TEST );

	LoadTextures();
	DefineQuadric();

	// registro de los eventos
    glutIdleFunc(Animar);
    glutReshapeFunc (reshapeevent);
	glutDisplayFunc( displayevent );
	glutKeyboardFunc(leerTeclado);			//Lectura de teclado
	glutIdleFunc( idleevent );
	glutMotionFunc(leerMov);
    glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;
}