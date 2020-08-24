#include <Servo.h>

Servo servoBase, servoJoint1, servoJoint2, servoWrist, servoClaw;

void setup() {
  servoBase.attach(53);
  servoJoint1.attach(52);
  servoJoint2.attach(51);
  servoWrist.attach(50);
  servoClaw.attach(49);
}

void loop() {
  servoBase.write(90);
  servoJoint1.write(90);
  servoJoint2.write(90);
  servoWrist.write(90);
  servoClaw.write(90);
}
