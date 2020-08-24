#include <Servo.h>

Servo servoleft, servoright, servohead; 

void setup() {
  servoleft.attach(53);
  servoright.attach(51);
  servohead.attach(52);
}

void loop() {
  servohead.write(90);
  servoleft.write(45);       // rotate counterclockwise full speed
  servoright.write(45);     // rotate clockwise full speed
  delay(2000);                          
  servoleft.write(90);          // stop
  servoright.write(90);
  delay(2000);
  servoleft.write(135);      // rotate clockwise full speed
  servoright.write(135);
  delay(2000);
  servoleft.write(90);          // stop
  servoright.write(90);
  delay(2000);
  //servoleft.write(80);     // rotate very slowly counterclockwise
  //servoleft.write(100);      // rotate very slowly clockwise
}
