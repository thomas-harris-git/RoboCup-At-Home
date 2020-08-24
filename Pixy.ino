#include <SPI.h>  
#include <Pixy.h>

const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 5;
const int rightBackward = 4;

int val = 250;
int stage = 1;

// This is the main Pixy object 
Pixy pixy;

void setup()
{
  Serial.begin(9600);
  
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);

  pixy.init();
}

void loop()
{ 
  static int i = 0;
  uint16_t blocks;
  char buf[32]; 
  
  // grab blocks!
  blocks = pixy.getBlocks();
  
  analogWrite (leftForward, 0);
  analogWrite (rightForward, 0);
  analogWrite (leftBackward, 0);
  analogWrite (rightBackward, 0);
  
  
  // If there are detect blocks, print them!
  if (blocks){
    /*
    if(stage == 1){
    if(pixy.blocks[0].x < 110){
       analogWrite (leftForward, val);
       analogWrite (rightBackward, val);
       delay(10);
     }
    else if(pixy.blocks[0].x > 125){
        analogWrite (rightForward, val);
        analogWrite (leftBackward, val);
        delay(10);
      }
     else{
       stage = 2;
       analogWrite (leftForward, 0);
       analogWrite (rightForward, 0);
       analogWrite (leftBackward, 0);
       analogWrite (rightBackward, 0);
       delay(500);
     }
    }
    if (stage == 2){
     if(110 < pixy.blocks[0].x < 125){
      if(pixy.blocks[0].height < 30){
        analogWrite (leftForward, val);
        analogWrite (rightForward, val);
        delay(10);
      }
      else{
        analogWrite (leftForward, 0);
        analogWrite (rightForward, 0);
        analogWrite (leftBackward, 0);
        analogWrite (rightBackward, 0);
        delay(10);
      }
     }
    }
    */
     
    i++;
    
    // do this (print) every 50 frames because printing every
    // frame would bog down the Arduino
    if (i%50==0)
    {
      sprintf(buf, "x: %d, y: %d, Hight: %d, Sig: %d\n\n",pixy.blocks[0].x, pixy.blocks[0].y, 
      pixy.blocks[0].height, pixy.blocks[0].signature);
      Serial.print(buf);
    }
  }  
}

