#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

VR myVR(10,11);

uint8_t buf[64];

int piezoPin = 8;

#define onRecord    (0)
#define offRecord   (1) 

void setup() {
  myVR.begin(9600);
}

void loop() {
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case onRecord:
        tone(piezoPin, 1000, 500);
        break;
      case offRecord:
        tone(piezoPin, 2000, 500);
        break;
    }
  }
}
