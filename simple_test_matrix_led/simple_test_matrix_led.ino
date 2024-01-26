#include <MD_MAX72xx.h>
#include <SPI.h>

#define CLK_PIN 13
#define DATA_PIN 11
#define CS_PIN 10

// #define LEFT 1
// #define RIGHT 0
// #define UP 2
// #define DOWN 3

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW,DATA_PIN,CLK_PIN,CS_PIN);

void setup() {
  // put your setup code here, to run once:
  mx.begin();

  // pinMode(RIGHT , INPUT);
  // pinMode(LEFT , INPUT);
  // pinMode(UP , INPUT);
  // pinMode(DOWN , INPUT);

}

int col =0 , row=0; 

void loop() {
  //Check For Inputs
  // int right = digitalRead(RIGHT);
  // int left = digitalRead(LEFT);
  // int up = digitalRead(UP);
  // int down = digitalRead(DOWN);
 
  // int pc = col , pr=row;
  // if(right == HIGH ){
  //   col = (col+1)%8;
  // }else if(left == HIGH ){
  //   col = col -1;
  //   if (col==-1)
  //     col=7;
  // }else if(down == HIGH ){
  //   row = row -1;
  //   if (row==-1)
  //     row=7;
  // }else if(up == HIGH){
  //   row = (row+1)%8;
  // }
  
  // mx.setPoint(pc, pr, false);

  mx.setPoint(0, 0, true);
  
  
  
}

