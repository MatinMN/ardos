#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#define MINPRESSURE 10
#define MAXPRESSURE 1000
// touch FIELDS
  uint8_t YP = A1;  // must be an analog pin, use "An" notation!
  uint8_t XM = A2;  // must be an analog pin, use "An" notation!
  uint8_t YM = 7;   // can be a digital pin
  uint8_t XP = 6;   // can be a digital pin
  uint8_t SwapXY = 0;

  int x= 0;
  int y = 0;
  uint16_t TS_LEFT = 161;
  uint16_t TS_RT  = 905;
  uint16_t TS_TOP = 164;
  uint16_t TS_BOT = 925;

  TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
  TSPoint tp;

  #define MINPRESSURE 15 
  #define MAXPRESSURE 1000

  
  bool loaded = false;
 int WIDTH = tft.width();
 int HEIGHT = tft.height();
 
 int state = 0 ; // 0 splash screen , 1 home , 2 app 
 int p_state = 0 ;

 // pong vars
 int pc_x = 0;
 int pc_y = 0;
 int player_x = 0;
 int player_y = 0;
 int pc_lx = 0;
 int pc_ly = 0;
 int player_lx = 0;
 int player_ly = 0;
 int ball_xspeed = 4;
 int ball_yspeed = 4;
 int pc_score = 0;
 int player_score = 0;
 int ball_x = 0;
 int ball_y = 0;
 int ball_lx = 0;
 int ball_ly = 0;
 // graph app vars
 char *graph_mode = 0;
void setup()
{

   
    // put your setup code here, to run once:
    Serial.begin(9600);
    tft.reset();
    uint16_t identifier = tft.readID();
    Serial.print("ID = 0x");
    Serial.println(identifier, HEX);
    if (identifier == 0xEFEF) identifier = 0x9486;
    tft.begin(identifier);
    tft.fillScreen(0xFFFF);
    
    // set the state
     ChangeState(0);
}

void readResistiveTouch(void)
{
    tp = ts.getPoint();
    pinMode(YP, OUTPUT);      //restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);   //because TFT control pins
    digitalWrite(XM, HIGH);
}



// SPLASH SCREEN
void Load() {
  tft.fillScreen(0x0000);
  //

   // CHANGE WHERE IT'S DRAWN
    tft.setTextColor(0xFFFF);
    tft.setCursor(80,150);
    tft.setTextSize(2);
    tft.print("WELCOME");
   // tft.print("LOADING...");

    delay(1000);
   
   ChangeState(1); // change state to state 1 (home screen)
  //
}

// TOP BAR 
void DrawFrame(char *n) { // n is name of the screen line home , setting ...
   // FRAME 
    tft.fillScreen(0xFFFF);
    tft.fillRect(0,0,WIDTH,15,0x0000);

    // CHANGE WHERE IT'S DRAWN
    tft.setTextColor(0xFFFF);
    tft.setCursor(0,0);
    tft.setTextSize(1.5);
    tft.print(n);
    if (state > 1){
    tft.setCursor(205,3);
    tft.setTextSize(1.5);
    tft.print("Back");
    }
    tft.setCursor(0, 0); // CHANGE DRAW CONTEXT BACK TO (0,0)
}

void DrawIcons(){

  //char *titles[] = {"Snake","pong,"calc","Setting"};
  // hardcoding all the Icons and buttons 15px margin 15 px padding 
   tft.setTextColor(0x0000);
   tft.drawRect(15,40,60,60,0x0000); 
   //tft.drawLine(20,80,60,45,0x0000);
    tft.drawRect(37,55,10,5,0x0000); 
    tft.drawRect(35,60,20,30,0x0000); 
    tft.setCursor(15,105);
    tft.setTextSize(1.5);
    tft.print("Paint");
    tft.setCursor(0,0);
    
  tft.drawRect(90,40,60,60,0x0000);
  tft.drawRect(100,50,7,40,0x0000);
  tft.drawRect(130,50,7,40,0x0000);
  tft.drawCircle(115,70,5,0x0000);
    tft.setCursor(90,105);
    tft.setTextSize(1.5);
    tft.print("Pong");
    tft.setCursor(0,0);
    
  tft.drawRect(165,40,60,60,0x0000);
  tft.setCursor(175,65);
    tft.setTextSize(2);
    tft.print("Y=X");
    tft.setCursor(0,0);
    tft.setCursor(165,105);
    tft.setTextSize(1.5);
    tft.print("Calculator");
    tft.setCursor(0,0);
    
    // 
    
  tft.drawRect(15,125,60,60,0x0000);
  tft.setCursor(30,140);
    tft.setTextSize(4);
    tft.print("R");
    tft.setCursor(0,0);
    tft.setCursor(15,190);
    tft.setTextSize(1.5);
    tft.print("Random");
    tft.setCursor(0,0);
  //tft.drawRect(90,125,60,60,0x0000);
  //tft.drawRect(165,125,60,60,0x0000);

}

// CHANGE THE CURRENT STATE 
void ChangeState(int s){
  state = s;
  loaded = false;
  if (s == 0){
    //DrawFrame("Spash Screen");
    Load();
    return;
   }else if (s == 1){
    DrawFrame("Home");
    DrawIcons();
    }
    else if (s == 2){ // snake 
    DrawFrame("Paint");
    }
    else if (s == 3){ // pong
    DrawFrame("Pong");
    } else if (s == 4){ // pong
    DrawFrame("Graph");
    }else if (s == 5){ // pong
    DrawFrame(graph_mode);
    }
    else if (s == 6){ // pong
    DrawFrame("Random");
    }
  
}


void LoadMenu(){

  uint16_t tx, ty;  //screen coordinates
  readResistiveTouch();
  // handle touch
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        tx = map(tp.x, TS_LEFT , TS_RT ,  0, tft.width());
        ty = map(tp.y, TS_TOP , TS_BOT ,  0, tft.height());
         // Serial.print("tx: ");
         // Serial.println(tx);
          // button one hard codding all the touch LOL
          if( tx>15 && tx<75 && ty>40 && ty<70){ // button one 
           ChangeState(2);
            }else if( tx>90 && tx<159 && ty>40 && ty<70){ // button two 
           ChangeState(3);
            }else if( tx>165 && tx<225 && ty>40 && ty<70){ // button two 
           ChangeState(4);
            } else if( tx>15 && tx<76 && ty>125 && ty<225){ // button two 
           ChangeState(6);
            }    
  } 
}

void LoadPaint(){
  // worst and simplest paint app ever (I'm lazy af)
  uint16_t tx, ty;  //screen coordinates
  readResistiveTouch();
  // handle touch
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        tx = map(tp.x, TS_LEFT , TS_RT ,  0, tft.width());
        ty = map(tp.y, TS_TOP , TS_BOT ,  0, tft.height());
        if (ty>15){
        tft.fillCircle(tx,ty,10,0x0000); 
        }
  } 
  
  
  
}

double CalculateBallAngle(int x,int xx ){
    return 5 *( (double) (x-xx)/30);
}

void pong_init(){
    DrawFrame("Pong");
    pc_x = tft.width()/2 - 30 ;
    pc_y = 15 ;
    pc_lx = pc_x;
    pc_ly = pc_y;
    ball_x =tft.width()/2;
    ball_y = tft.height()/2  +15;
    ball_lx = ball_x;
    ball_ly = tft.height()/2  +15;
    player_x = pc_x;
    player_y = tft.height()-15;
    player_lx = player_x;
    player_ly = player_y;
    ball_xspeed = random(-5,5);
    ball_yspeed = 4;
    loaded = true;
    tft.drawRect(player_lx,player_ly,60,20,0x0000); 
    tft.drawRect(player_x,player_y,60,20,0x0000); 
    tft.drawRect(pc_lx,pc_ly,60,20,0x0000); 
    tft.drawRect(pc_x,pc_y,60,20,0x0000); 
    tft.drawCircle(ball_x,ball_y,10,0x0000); 
    tft.drawRect(player_lx,player_ly,60,30,0x0000); 
    tft.drawLine(0,tft.height()/2+15,tft.width(),tft.height()/2+15,0x0000); 
    tft.setTextColor(0x0000);
    tft.setTextSize(1.5);
    tft.setCursor(10 ,tft.height()/2 );
    tft.print(pc_score);
    tft.setCursor(10,tft.height()/2 +20);
    tft.print(player_score);
    tft.setCursor(0,0);
    delay(1000);
}

void LoadPong(){
  if ( loaded == false ){
    // load game peddles 
    // ball size is 10 paddle size is 60 by 20 
    player_score = 0;
    pc_score = 0;
    pong_init();
   }
   // draw 
   
   tft.drawRect(player_lx,player_ly,60,20,0xFFFF); 
   tft.drawRect(player_x,player_y,60,20,0x0000); 
   tft.drawRect(pc_lx,pc_ly,60,20,0xFFFF); 
   tft.drawRect(pc_x,pc_y,60,20,0x0000); 
   tft.drawCircle(ball_lx,ball_ly,10,0xFFFF); 
   tft.drawCircle(ball_x,ball_y,10,0x0000); 
   //tft.drawRect(player_lx,player_ly,60,30,0x0000); 
   tft.drawLine(0,tft.height()/2+15,tft.width(),tft.height()/2+15,0x0000); 
   tft.setTextColor(0x0000);
    tft.setTextSize(1.5);
    tft.setCursor(10 ,tft.height()/2 );
    tft.print(pc_score);
    tft.setCursor(10,tft.height()/2 +20);
    tft.print(player_score);
    tft.setCursor(0,0);
   //start the match 
   pc_lx = pc_x;
   pc_ly = pc_y;
   ball_lx = ball_x;
   ball_ly = ball_y;
   player_lx = player_x;
   player_ly = player_y;
   // move the ball
   ball_x += ball_xspeed;
   ball_y += ball_yspeed;
   //pc and ball
   if (ball_x < 10 || ball_x >tft.width()-10 ){ball_xspeed = - ball_xspeed; ball_x +=ball_xspeed; }
   if (ball_y < 25 ){  player_score++; ball_x =tft.width()/2;
    ball_y = tft.height()/2  +15; pong_init();}
   if ( ball_y >tft.height()-10 ){pc_score++; ball_x =tft.width()/2;
    ball_y = tft.height()/2  +15; pong_init();}
   if(ball_x > pc_x + 30){
      pc_x +=3;
       if(ball_x < pc_x + 30){pc_x -=1;}
    }else{
      pc_x -=3;
      if(ball_x > pc_x + 30){pc_x +=1;}
     }
   if (pc_x < 0 ){pc_x = 0;}
   if (pc_x > tft.width()-60 ){pc_x = tft.width()-60;}
   if (abs(ball_x - (pc_x + 30) )<40 && abs(ball_y - (pc_y + 10))<20){
         ball_xspeed =  CalculateBallAngle(ball_x,pc_x+30) ;
         ball_yspeed = - ball_yspeed;
   }

   // player

  uint16_t tx, ty;  //screen coordinates
  readResistiveTouch();
  // handle touch
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        tx = map(tp.x, TS_LEFT , TS_RT ,  0, tft.width());
        ty = map(tp.y, TS_TOP , TS_BOT ,  0, tft.height());
       //Serial.print("tx: ");
         // Serial.println(tx);
         player_x = tx-30;
  } 
   
   if (player_x < 0 ){player_x = 0;}
   if (player_x > tft.width()-60 ){player_x = tft.width()-60;}
   if (abs(ball_x - (player_x + 30) )<40 && abs(ball_y - (player_y + 10))<20){
      ball_xspeed =  CalculateBallAngle(ball_x,player_x+30) ;
      ball_yspeed = - ball_yspeed;
   }
   
  
}

void LoadGraph() {
  tft.setTextColor(0x0000);
   tft.drawRect(15,40,60,60,0x0000); 
    tft.setCursor(15,105);
    tft.setTextSize(1.5);
    tft.print("y=x");
    tft.setCursor(0,0);
    
  tft.drawRect(90,40,60,60,0x0000);
    tft.setCursor(90,105);
    tft.setTextSize(1.5);
    tft.print("y=x^2");
    tft.setCursor(0,0);
    
  tft.drawRect(165,40,60,60,0x0000);
    tft.setCursor(165,105);
    tft.setTextSize(1.5);
    tft.print("sin(x)");
    tft.setCursor(0,0);
    
    // handle touch 
    uint16_t tx, ty;  //screen coordinates
  readResistiveTouch();
  // handle touch
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        tx = map(tp.x, TS_LEFT , TS_RT ,  0, tft.width());
        ty = map(tp.y, TS_TOP , TS_BOT ,  0, tft.height());
          //Serial.print("tx: ");
          //Serial.println(tx);
          // button one hard codding all the touch LOL
          if( tx>15 && tx<75 && ty>40 && ty<70){ // button one 
          graph_mode = "y=x";
          ChangeState(5);
            }else if( tx>90 && tx<159 && ty>40 && ty<70){ // button two 
           graph_mode = "y=x^2";
           ChangeState(5);
            }else if( tx>165 && tx<225 && ty>40 && ty<70){ // button two 
           graph_mode = "sin(x)";
           ChangeState(5);
            }    
  } 
  
 }


void DrawGraph(){
  tft.drawLine(0,tft.height()/2,tft.width(),tft.height()/2,0x0000); 
  tft.drawLine(tft.width()/2,0,tft.width()/2,tft.height(),0x0000); 
if (graph_mode == "y=x"){
   for (double i=-240;i<240;i++){
    tft.drawCircle((i+tft.width()/2),(-i+(tft.height()/2)),2,0x0000); 
    //delay(2);
  }
  
}else if (graph_mode == "y=x^2"){
   for (double i=-240;i<240;i++){
    tft.drawCircle((i+tft.width()/2),tft.height()/2 - ( i*i )/50,2,0x0000); 
    //delay(2);
  }
}else if (graph_mode == "sin(x)"){
    for (double i=-240;i<240;i++){
    tft.drawCircle(((i+tft.width()/2)),tft.height()/2+50*sin(50*i*3.14),2,0x0000); 
    //delay(2);
  }
}
  
  
}

void LoadRandom(){
  
char *randoms[]  = {"heh","I'm your father","Hello","21"," hello","call me maybe","faster","slower","daaaaammmn","calm down","test","boop","noooo","mother knows best","sword","bow","...","I hate you"};
    if(loaded == false){
     tft.setTextColor(0x0000);
    tft.setCursor(0,105);
    tft.setTextSize(2);
    tft.print(randoms[random(0,18)]);
    tft.setCursor(0,0);
    loaded = true;
     }
    
  
  
}

// GAME LOOP only used for games
void loop()
{
    if (state == 2) {LoadPaint();}
   if (state == 3) {LoadPong();}
   if (state == 4) {LoadGraph();}
   if (state == 5) {DrawGraph(); }
   if (state == 6) {LoadRandom();}
   if (state == 1) {LoadMenu();}else {
       // handle touch 
    uint16_t tx, ty;  //screen coordinates
  readResistiveTouch();
  // handle touch
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
        tx = map(tp.x, TS_LEFT , TS_RT ,  0, tft.width());
        ty = map(tp.y, TS_TOP , TS_BOT ,  0, tft.height());
          //Serial.print("tx: ");
          //Serial.println(tx);
          // button one hard codding all the touch LOL
          if( tx>180 && tx<240 && ty>0 && ty<40){ // button one 
          ChangeState(1);}
  } 
    
    
   }
   
  
  delay(10);
}
