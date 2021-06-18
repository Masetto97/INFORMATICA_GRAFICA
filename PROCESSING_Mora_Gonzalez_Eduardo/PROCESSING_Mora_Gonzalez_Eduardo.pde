import processing.sound.*;  //Para evitar el error se debe instalar dicha libreria. Sketch -> Importar Libreria -> Añadir libreria -> Sound

/*
  VARIABLES GLOBALES
*/

//Variables para el archivo de Audio
SoundFile file;
String audioName = "Game.mp3";

//Variables para controlar los fondos, las pantallas, las naves...
String path;
int screen; 
PImage background_menu, background_inGame, background_gameOver, background_gameWin, startButtonImage, ship;

//Variables para controlar las posiciones y los puntos
int posXship, posYship, sizeXship, sizeYship, points;

//Variables para controlar el tiempo
int timer, seconds, minutes, timerMax;

//Lista de Balas y enemigos
ArrayList<Bullet> bullets;
ArrayList<Sith> siths;
ArrayList<Destroyer> destroyers; 

//Controlar el numero de balas disponibles
int numeroTotalBalas;


/*
  METODOS PRINCIPALES
*/


void setup(){
  
  //Declaramos el tamaño de la pantalla y la escena que queremos
  size(1280,920);
  screen=1;
  
  //Cargamos los fondos y el boton de inicio
  background_menu = loadImage("background_2.jpg");
  background_inGame = loadImage("background_1.jpg");
  background_gameOver= loadImage("background_3.jpg");
  background_gameWin = loadImage("background_4.jpg");
  startButtonImage = loadImage("start.jpg");
  
  //Cargamos la Nave principal
  ship= loadImage("nave.png");
  
  posXship= (int)((width*3)/4);
  posYship= (int)((width/2)-width/8);
  sizeXship= (int)width/8;
  sizeYship= (int)height/8;
  
  //Inizializamos la listas de Balas y Enemigos 
  bullets = new ArrayList<Bullet>(); 
  siths = new ArrayList<Sith>();
  destroyers = new ArrayList<Destroyer>();
  
  //Inizializamos el Timer
  timer= 0;
  
  //Cargamos y reproducimos la musica
  path= sketchPath(audioName);
  file= new SoundFile(this, path);
  file.play(); 
  
  //Numero maximo de balas en la pantalla
  numeroTotalBalas = 3;
}


void draw(){
  
  //Switch para alternar entre las distintas pantallas del juego
  switch(screen){
    case 1:   menu();     break;  //Pantalla de Inicio del Juego
    case 2:   game();     break;  //Pantalla del Juego
    case 3:   gameOver(); break;  //Pantalla de Fin del Juego
    case 4:   gameWin(); break;   //Pantalla de has ganado
    default:  menu();     break;
   }
  
}


//Pantalla de Inicio del Juego
void menu(){
 
  //Cargamos el Fondo
  image(background_menu,0,0,width,height);
  
  //Cargamos la imagen del boton de Inicio
  image(startButtonImage,width/2 - width/16,height/2 - height/16,width/8,height/8);
  
  //Comprobamos que en el rectangualo del boton de Inicio se presiona con el ratón para cargar o no la segunda pantalla
  if(mouseX > width/2 - width/16 && mouseX <width/2 - width/16 + width/8 && mouseY>height/2 - height/16 && mouseY<height/2 - height/16 + height/8 && mousePressed){
    screen=2;
  }
}


//Pantalla principal del Juego
void game(){

  //Inizializamos el Timer
  timerMax=50; 
  timer=int (millis()/1000);
  calculateTimers();
  
  //Cargamos la imagen del fondo
  image(background_inGame,0,0,width,height+height/8);
  
  //Llamada a los distintos metodos que implementan la logica de juego
    shipMovementandShoot();
    
    ship();
    
    createSiths();
    
    createDestroyer();
    
    showbullets();
    
    showSith();
    
    showDestroyer();
    
    check();
    
    die();
  
  //Mostramos en la pantalla los puntos y el tiempo
  fill(255);
  textSize(40);
  text("Tiempo: " +minutes+":"+ seconds, width/10, height-50);
  text("Puntos: " +points,width*8/10 , height-50);
  text("Balas: "  +numeroTotalBalas, width*5/10 , height-50);
  
  //Si el tiempo es 0 mostramos la pantalla que has perdido
  if(timerMax == 0)  screen=4;
  
}


//Metodo para calcular los minutos y segundos que quedan
void calculateTimers(){
    timerMax-= timer;
    seconds= timerMax%60;
    minutes= timerMax/60;
}


//Pantalla Cuando Pierdes la partida
void gameOver(){
  image(background_gameOver,0,0,width,height); 
}


//Pantalla Cuando Pierdes la partida
void gameWin(){
  image(background_gameWin,0,0,width,height);
  fill(255);
  textSize(40);
  text("    GANASTES CON : " +points + " PUNTOS",width/10 , height-50);
}


//Cargar la posicion de la Nave Principal
void ship(){
  image(ship,posXship,posYship,sizeXship,sizeYship); 
}


//Control de las teclas para mover la Nave Principal
void shipMovementandShoot(){

  if(keyCode==UP && keyPressed && posYship>0){
      posYship-=5;  
   }
  else if(keyCode== DOWN && keyPressed &&(posYship+sizeYship<height*3/4)){
    posYship+=5; 
  }
  if( keyCode==LEFT && keyPressed){
    
    //Para controlar que solo existan 3 balas en la pantalla
    if (numeroTotalBalas > 0 && numeroTotalBalas <=3 ){
      numeroTotalBalas-=1;
      bullets.add(new Bullet(posXship,(posYship+(sizeYship/2))));
    }
  }
  
}


//Mostrar las naves enemigas
void showDestroyer(){
  
  for(int i= destroyers.size()-1; i>=0 ; i-- ){//Recorremos la Lista de enemigos
  
    //Obtenemos la nave enemiga, la movemos y la mostramos
    Destroyer d = destroyers.get(i);
    d.move();
    d.Display();
    
    if(d.Destroy()){ //Si le ha dado una bala se elimina de la Lista
      destroyers.remove(i);
    } 
  }
}


//Mostrar las naves enemigas 
void showSith(){
  
  for(int i= siths.size()-1; i>=0 ; i-- ){//Recorremos la Lista de enemigos
  
    //Obtenemos la nave enemiga, la movemos y la mostramos
    Sith s= siths.get(i);
    s.move();
    s.Display();
    
    if(s.Destroy()){ //Si le ha dado una bala se elimina de la Lista
      siths.remove(i);
    } 
  }
}


//Mostrar las Balas lanzadas
void showbullets(){
  for(int i= bullets.size()-1; i>=0; i--){//Recorremos la Lista de Balas
  
    //Obtenemos la bala, la movemos y la mostramos  
    Bullet b= bullets.get(i);
    b.move();
    b.Display();
    
    if(b.Destroy()){ //Si llega al final o le da a un enemigo se destruye 
      bullets.remove(i);
      
      if(numeroTotalBalas < 3){
        numeroTotalBalas++;
      }
    }
  }
}


//Metodo para crear los enemigos
void createSiths(){
 
 //Cada 4 segundos aparece un nuevo enemigo
 if((millis()/100)%40==0){
   
   //Le asignamos una posicion aleatoria y los añadimos a la lista de enemigos
   float r= random(0,(((height*3)/4)-25)); 
   siths.add(new Sith(int(0), int(r)));
   
 }
}


//Metodo para crear los enemigos destroyer
void createDestroyer(){
 
 //Cada 5 segundos aparece un nuevo enemigo
 if((millis()/100)%50==0){
   
   //Le asignamos una posicion aleatoria y los añadimos a la lista de enemigos
   float r= random(0,(((height*3)/4)-25)); 
   destroyers.add(new Destroyer(int(0), int(r)));
   
 }
}


//Metodo para comprobar si hemos dado a un enemigo
void  check(){
  
  //Recorremos la lista de balas
  for (int i = bullets.size()-1; i >= 0; i--) {
    
      //Obtenemos la posicion de las balas
      Bullet b=bullets.get(i);
      int positionsBulletX=b.get_PosX();
      int positionsBulletY=b.get_PosY();
      
      //Recorremos la lista de enemigos
      for (int y = siths.size()-1; y >= 0; y--) {
        
        //Obtenemos la posicion de los enemigos
        Sith s=siths.get(y);
        int positionSithX=s.get_PosX();
        int positionSithY=s.get_PosY();
        
        //Si coincide eliminamos la bala y el enemigo y sumamos un punto
        if(positionsBulletX >= positionSithX && positionsBulletX <= positionSithX+int(width/8)){
          if(positionsBulletY >= positionSithY && positionsBulletY<= positionSithY+int(width/8)){
            bullets.remove(i);
            siths.remove(y); 
            points+=1;
            
            if(numeroTotalBalas < 3){
              numeroTotalBalas++;
            }
          }
        }
      }
      
      //Recorremos la lista de enemigos destroyer
      for (int y = destroyers.size()-1; y >= 0; y--) {
        
        //Obtenemos la posicion de los enemigos
        Destroyer d=destroyers.get(y);
        int positionDestroyerX=d.get_PosX();
        int positionDestroyerY=d.get_PosY();
        
        //Si coincide eliminamos la bala y el enemigo y sumamos un punto
        if(positionsBulletX >= positionDestroyerX && positionsBulletX <= positionDestroyerX+int(width/8)){
          if(positionsBulletY >= positionDestroyerY && positionsBulletY<= positionDestroyerY+int(width/8)){
            
            if(d.get_life() == 0){
              bullets.remove(i);
              destroyers.remove(y); 
              points+=2;
            }else{
              d.bullsLife();
              bullets.remove(i);
            }
            if(numeroTotalBalas < 3){
              numeroTotalBalas++;
            }
          }
        }
      }
    }
}


//Metodo para comprobar si un enemigo nos ha dado
void die(){
  
  //Recorremos la lista de enemigos
  for (int i = siths.size()-1; i >= 0; i--) {
    
    //Obtenemos la posicion de los enemigos
    Sith s=siths.get(i);
    int positionSithX=s.get_PosX();
    int positionSithY=s.get_PosY();
    
     //Si coincide con nuestra posicion, hemos perdido
     if((positionSithX+int(width/8))>=posXship  && positionSithX <= posXship+ sizeXship ){
       if((positionSithY + width/8)>=posYship && positionSithY<= posYship+ sizeYship){
         screen=3; 
       }
          
     }
    
  }
  
  //Recorremos la lista de enemigos destroyer
  for (int i = destroyers.size()-1; i >= 0; i--) {
    
    //Obtenemos la posicion de los enemigos
    Destroyer d=destroyers.get(i);
    int positionDestroyerX=d.get_PosX();
    int positionDestroyerY=d.get_PosY();
    
     //Si coincide con nuestra posicion, hemos perdido
     if((positionDestroyerX+int(width/8))>=posXship  && positionDestroyerX <= posXship+ sizeXship ){
       if((positionDestroyerY + width/8)>=posYship && positionDestroyerY<= posYship+ sizeYship){
         screen=3; 
       }
          
     }
    
  }
}


//Clase para gestionar lo relacionado con las balas
class Bullet{
  
  //Atributos de las balas
  int posX, posY, velocity, sizeX, sizeY;
  
  //Constructor
  Bullet (int PosX, int PosY){
   posX = PosX; 
   posY = PosY; 
   velocity= 30; //Le asignamos una velocidad de 30
   sizeX =10; 
   sizeY =8; 
  }
  
  //Metodo para mover las balas por la pantalla
  void move(){
    posX-=velocity; 
  }
  
  //Metodo para eliminar la bala si ha salido de la pantalla
  boolean Destroy(){
    if(posX<0){
        return true;
    }else{
      return false;
    }
  }
  
  int get_PosX(){
   return posX;
  }
  int get_PosY(){
    return posY;
  }
  
  //Metodo para dibujar las balas
  void Display(){
    fill(2, 188, 252);
    noStroke(); 
    rect(posX,posY,sizeX,sizeY); 
  }
}


//Clase para gestionar lo relacionado con los enemigos
class Sith{
  
  //Atributos de los enemigos
  int posX, posY, vX, vY, sizeX, sizeY;
  
  //Imagen de la nave enemiga
  PImage Vader;
  
  //Constructor
  Sith(int PosX, int PosY){
    posX = PosX;
    posY = PosY;
    vX= 6;
    vY= 6; 
    sizeX = width/8;
    sizeY = width/8;
    Vader= loadImage("SithShip.png"); 
  }
  
  //Metodo para mover las naves enemigas
  void move(){
    if(posY<=0  ){
      vY=+6;   
    }
   else if( (posY+width/8 )>= ((height*3)/4)+10){
       vY=-6;
   }
   
    posX+= vX;
    posY+= vY;
  }
  
  //Metodo para eliminar la nave si le ha dado una bala
  boolean Destroy(){
    if(posX>= width){
        return true;    
    }else{
      return false;
    }
  }
  
  int get_PosX(){
    return posX;
  }
  
  int get_PosY(){
    return posY; 
  }
  
  //Metodo para dibujar las naves enemigas
  void Display(){
    image(Vader,posX, posY, sizeX, sizeY); 
  }
  
}


//Clase para gestionar lo relacionado con los enemigos dobles
class Destroyer{
  
  //Atributos de los enemigos
  int posX, posY, vX, vY, sizeX, sizeY, life;
  
  //Imagen de la nave enemiga
  PImage Imperial;
  
  //Constructor
  Destroyer(int PosX, int PosY){
    posX = PosX;
    posY = PosY;
    vX= 6;
    vY= 6; 
    sizeX = width/8;
    sizeY = width/8;
    Imperial= loadImage("DestroyShip.png");
    life = 2; //Numero de disparos que hace falta para destruirlo
  }
  
  //Metodo para mover las naves enemigas
  void move(){
    if(posY<=0  ){
      vY=+6;   
    }
   else if( (posY+width/8 )>= ((height*3)/4)+10){
       vY=-6;
   }
   
    posX+= vX;
    posY+= vY;
  }
  
  //Metodo para eliminar la nave si le ha dado una bala y no tiene vida
  boolean Destroy(){
    if(posX>= width && life == 0){
        return true;    
    }else{
      return false;
    }
  }
  
  int get_PosX(){
    return posX;
  }
  
  int get_PosY(){
    return posY; 
  }
  
  int get_life(){
    return life; 
  }
  
  void bullsLife(){
    life = life -1;
  }
  
  //Metodo para dibujar las naves enemigas
  void Display(){
    image(Imperial,posX, posY, sizeX, sizeY); 
  }
  
}
