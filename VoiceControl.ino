#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

VR myVR(10,11);
SoftwareSerial GPRS(22, 23);

uint8_t records[7]; // save record
uint8_t buf[64];

#define forward (0)
#define back (1)
#define left (2)
#define right (3)
#define sendtext (4)

const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 5;
const int rightBackward = 4;

void setup() {
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);
  
  myVR.begin(9600);
  GPRS.begin(9600);
  GPRS.println("AT+CMGF=1");
  delay(1000);
}

void loop() {
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case forward:
        analogWrite (leftForward, 210);
        analogWrite (rightForward, 250);
        delay(4000);
        break;
      case back:
        digitalWrite(leftForward , LOW);
        digitalWrite(leftBackward , HIGH);
        digitalWrite(rightForward , LOW);
        digitalWrite(rightBackward , HIGH);
        delay(1000);
        break;
      case left:
        digitalWrite(leftForward , LOW);
        digitalWrite(leftBackward , HIGH);
        digitalWrite(rightForward , HIGH);
        digitalWrite(rightBackward , LOW);
        delay(1000);
        break;
      case right:
        digitalWrite(leftForward , HIGH);
        digitalWrite(leftBackward , LOW);
        digitalWrite(rightForward , LOW);
        digitalWrite(rightBackward , HIGH);
        delay(1000);
        break;
      case sendtext:
        sendSMS();
        break;
    }
  }
  digitalWrite(leftForward , LOW);
  digitalWrite(leftBackward , LOW);
  digitalWrite(rightForward , LOW);
  digitalWrite(rightBackward , LOW);
}

void sendSMS() { 
  GPRS.println("AT+CMGS=\"07585706676\"");
  delay(500);
  GPRS.print("This is a text from your robot");
  GPRS.write( 0x1a ); // ctrl+Z character
  delay(500);
}
