#include <FastLED.h>
#include <stdio.h>


//Define The Snake as a Struct
typedef struct Snake Snake;
struct Snake{
  int head[2];     // the (row, column) of the snake head
  int body[40][2]; //An array that contains the (row, column) coordinates
  int len;         //The length of the snake 
  int dir[2];      //A direction to move the snake along
};

//Define The Apple as a Struct
typedef struct Apple Apple;
struct Apple{
  int rPos; //The row index of the apple
  int cPos; //The column index of the apple
};

//MAX72XX led Matrix

#define LED_PIN  4

#define COLOR_ORDER GRB
#define CHIPSET     WS2811



#define BRIGHTNESS 250

#define LEFT 1
#define RIGHT 0
#define UP 2
#define DOWN 3

Snake snake = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};//Initialize a snake object
Apple apple = {(int)random(0,8),(int)random(0,8)};//Initialize an apple object

//Variables To Handle The Game Time
float oldTime = 0;
float timer = 0;
float updateRate = 3;

const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

int i,j;//Counters
void setup() {
  // put your setup code here, to run once:

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  pinMode(RIGHT , INPUT);
  pinMode(LEFT , INPUT);
  pinMode(UP , INPUT);
  pinMode(DOWN , INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  float deltaTime = calculateDeltaTime();
  timer += deltaTime;

  //Check For Inputs
  int right = digitalRead(RIGHT);
  int left = digitalRead(LEFT);
  int up = digitalRead(UP);
  int down = digitalRead(DOWN);
  
  if(left == HIGH && snake.dir[1]==0){
    snake.dir[0] = 0;
    snake.dir[1] = -1;
  }else if(right == HIGH && snake.dir[1]==0){
    snake.dir[0] = 0;
    snake.dir[1] = 1;
  }else if( up== HIGH && snake.dir[0]==0){
    snake.dir[0] = -1;
    snake.dir[1] = 0;
  }else if(down == HIGH && snake.dir[0]==0){
    snake.dir[0] = 1;
    snake.dir[1] = 0;
  }
  
  //Update
  if(timer > 1000/updateRate){
    timer = 0;
    Update();
  }
    
}

float calculateDeltaTime(){
  float currentTime = millis();
  float dt = currentTime - oldTime;
  oldTime = currentTime;
  return dt;
}

void reset(){
  for(i=0;i<8;i++){
      for (j=0;j<8;j++){
        leds[ XY(j, i)]  = CHSV( 0, 0, 0);
      }
  }
}
void Update(){
  reset();//Reset (Clear) the 8x8 LED matrix
  
  int newHead[2] = {snake.head[0]+snake.dir[0], snake.head[1]+snake.dir[1]};

  //Handle Borders
  if(newHead[0]==8){
    newHead[0]=0;
  }else if(newHead[0]==-1){
    newHead[0] = 7;
  }else if(newHead[1]==8){
    newHead[1]=0;
  }else if(newHead[1]==-1){
    newHead[1]=7;
  }
  
  //Check If The Snake hits itself
   for(j=0;j<snake.len;j++){
    if(snake.body[j][0] == newHead[0] && snake.body[j][1] == newHead[1]){
      //Pause the game for 1 sec then Reset it
      delay(1000);
      snake = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};//Reinitialize the snake object
      apple = {(int)random(0,8),(int)random(0,8)};//Reinitialize an apple object
      return;
    }
  }

  //Check if The snake ate the apple
  if(newHead[0] == apple.rPos && newHead[1] ==apple.cPos){
    snake.len = snake.len+1;
    apple.rPos = (int)random(0,8);
    apple.cPos = (int)random(0,8);
  }else{
    removeFirst();//Shifting the array to the left
  }
  
  snake.body[snake.len-1][0]= newHead[0];
  snake.body[snake.len-1][1]= newHead[1];
  
  snake.head[0] = newHead[0];
  snake.head[1] = newHead[1];
  
  //Update the pic Array to Display(snake and apple)

  for(i=0;i<8;i++){
      for (j=0;j<8;j++){
        leds[ XY(j, i)]  = CHSV( 0, 0, 0);
      }
  }

  int x,y;
  for(j=0;j<snake.len;j++){
    int x = snake.body[j][1], y = snake.body[j][0];
    if (y%2 ==1)
      x = 7-x;
    leds[ XY(x, y)]  = CHSV( 255, 0, 255);
  }
  if (apple.rPos %2 == 1) 
    x = 7 - apple.cPos;
  else 
    x=apple.cPos;
  leds[ XY(x, apple.rPos)]  = CHSV( 0, 255, 255);

  FastLED.show();
  
}

void removeFirst(){
  for(j=1;j<snake.len;j++){
    snake.body[j-1][0] = snake.body[j][0];
    snake.body[j-1][1] = snake.body[j][1];
  }
}