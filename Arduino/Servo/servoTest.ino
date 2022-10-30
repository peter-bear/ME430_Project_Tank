#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  myservo.attach(13);
} 

void loop() {
    myservo.write(0);
} 