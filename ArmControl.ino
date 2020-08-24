#include <Servo.h>

Servo Base, Joint1, Joint2, Claw;

void setup() {
  Base.attach(46);
  Joint1.attach(42);
  Joint2.attach(38);
  Claw.attach(34);
  rest();
}

void loop() {
  delay(20000);
  grab();
}

void rest() {
  Base.write(60);
  delay(500);
  Joint1.write(140);
  delay(10);
  Claw.write(180);
  delay(500);
  Joint2.write(180);
  delay(10);
}

void grab() {
  for(int i=0; i<90; i++){
    Joint2.write(180-i);
    delay(100);
  }  
  delay(500);
  Claw.write(90);
  delay(10);
  for(int i=0; i<40; i++){
    Base.write(60+i);
    Joint1.write(140+i);
    delay(100);
  }
  delay(500);
  Claw.write(130);
  delay(500);
  for(int i=0; i<40; i++){
    Base.write(100-i);
    Joint1.write(180-i);
    delay(100);
  }
  delay(9000000);
}
