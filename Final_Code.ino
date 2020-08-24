#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <Wire.h>
#include "ds3231.h"
#include <Servo.h>
#include <SPI.h>  
#include <Pixy.h>

#define BUFF_MAX 128

Servo Base, Joint1, Joint2, Claw;

uint8_t time[8];
char recv[BUFF_MAX];
unsigned int recv_size = 0;

VR myVR(10,11);
SoftwareSerial GPRS(22, 23);

uint8_t records[7]; // save record
uint8_t buf[64];

int piezoPin = 8;

#define GetHelp (0)
#define ImLonely (1)
#define GetMedicine (2)

const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 5;
const int rightBackward = 4;

int val = 250; //Valocity of the motors
int stage, sig, h;

Pixy pixy;

void setup() {
  Base.attach(46);
  Joint1.attach(42);
  Joint2.attach(38);
  Claw.attach(34);
  rest();
  
  pixy.init();
  
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  memset(recv, 0, BUFF_MAX);

  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);
  
  myVR.begin(9600);
  GPRS.begin(9600);
  GPRS.println("AT+CMGF=1");
  delay(1000); //wait to allow GSM module time to start up
}

void loop() {
  char buff[BUFF_MAX];
   struct ts t;
    
   DS3231_get(&t);

#ifdef CONFIG_UNIXTIME //Configures the time layout
        snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %ld", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec, t.unixtime);
#else
        snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec);
#endif

  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){ //switch case for the three voice commands
      case GetHelp:
        tone(piezoPin, 1000, 500); //piezo makes a short tone
        GPRS.println("AT+CMGS=\"07585706676\""); //sets the number to send the text to
        delay(500);
        GPRS.print("Granny Needs Help!\nSent at: "); //message sent
        GPRS.println(buff); //adds the currrent time
        GPRS.write( 0x1a ); // ctrl+Z character
        delay(500); //delays to allow the GSM module time to act
        break;
      case ImLonely:
        tone(piezoPin, 1000, 500);
        GPRS.println("AT+CMGS=\"07585706676\"");
        delay(500);
        GPRS.print("Granny Is Lonely\nSent at: ");
        GPRS.println(buff);
        GPRS.write( 0x1a ); 
        delay(500);
        break;
      case GetMedicine:
        tone(piezoPin, 1000, 500);
        sig = 10; //sets the signiture number that the pixy cam looks for
        h = 33; //sets the hight so the robot knows when to stop
        searching(); //searches for the object
        Move(); //moves towards the object
        grab(); //performs grabbing procedure 
        sig = 28;
        h = 50;
        searching();
        Move();
        LetGo(); //places object down
        break;
    }
  }
}

void searching(){ //spins around on the spot until it spots the object
  while(1){
    uint16_t blocks;
    char buf[32]; 
    // grab blocks!
    blocks = pixy.getBlocks();
    
    analogWrite (leftForward, 200);
    analogWrite (rightBackward, 200);
    delay(10);
    
    // If there are detected blocks
    if (blocks){
      if (pixy.blocks[0].signature==sig){
        stage = 1;
        Stop();
        delay(10);
        break;
      }
    }
  }
}

void rest() { //set the resting position of the arm 
  Base.write(70);
  delay(500);
  Joint1.write(140);
  delay(10);
  Claw.write(180);
  delay(500);
  Joint2.write(180);
  delay(10);
}

void grab() { //performs grabbing procedure
  for(int i=0; i<90; i++){
    Joint2.write(180-i);
    delay(100);
  }  
  delay(500);
  Claw.write(90);
  delay(10);
  for(int i=0; i<30; i++){
    Base.write(70+i);
    Joint1.write(140+i);
    delay(100);
  }
  delay(500);
  Claw.write(130);
  delay(500);
  for(int i=0; i<30; i++){
    Base.write(100-i); 
    Joint1.write(170-i); 
    delay(100);
  }
  delay(2000);
}

void LetGo() { //places object down
  for(int i=0; i<30; i++){
    Base.write(70+i);
    Joint1.write(140+i);
    delay(100);
  }
  Claw.write(90);
  delay(500);
  for(int i=0; i<30; i++){
    Base.write(100-i); 
    Joint1.write(170-i); 
    delay(100);
  }
  for(int i=0; i<90; i++){
    Joint2.write(90+i);
    delay(100);
  }
  rest();
}

void Move() { //moves toward an object
  while(1){
    uint16_t blocks;
    char buf[32]; 
    
    // grab blocks!
    blocks = pixy.getBlocks();
    
    Stop();
    
    if (blocks){// If there are detected blocks
      if(stage == 1){ //step one line up with object
        if(pixy.blocks[0].x < 95){
         analogWrite (leftForward, val);
         analogWrite (rightBackward, val);
         delay(10);
       }
      else if(pixy.blocks[0].x > 98){
          analogWrite (rightForward, val);
          analogWrite (leftBackward, val);
          delay(10);
        }
       else{
         stage = 2;
         Stop();
         delay(500);
       }
      }
      if (stage == 2){ //step 2 move towards object
       if(95 < pixy.blocks[0].x < 98){
        if(pixy.blocks[0].height < h){
          analogWrite (leftForward, 210);
          analogWrite (rightForward, val);
          delay(10);
        }
        else{
          stage = 3;
          Stop();
          delay(10);
        }
       }
      }
      if (stage == 3){ //step 3 line up with object again
        if(pixy.blocks[0].x < 95){
         analogWrite (leftForward, val);
         analogWrite (rightBackward, val);
         delay(10);
       }
      else if(pixy.blocks[0].x > 98){
          analogWrite (rightForward, val);
          analogWrite (leftBackward, val);
          delay(10);
        }
       else{
         Stop();
         delay(500);
         break;
       }
      }
    }
  }
}

void Stop(){ //Stops the motors
  analogWrite (leftForward, 0);
  analogWrite (rightForward, 0);
  analogWrite (leftBackward, 0);
  analogWrite (rightBackward, 0);
}
