const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 5;
const int rightBackward = 4;

int val = 250;

void setup() 
{
  pinMode(leftForward , OUTPUT);
  pinMode(leftBackward , OUTPUT);
  pinMode(rightForward , OUTPUT);
  pinMode(rightBackward , OUTPUT);
}

void loop()
{
  analogWrite (leftForward,val);
  analogWrite (rightForward,val);
  delay(10); 
}
