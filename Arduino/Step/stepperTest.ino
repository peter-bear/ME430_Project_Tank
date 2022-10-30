#include <Stepper.h>

#define leftBtn 3
#define rightBtn 4

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
// for your motor


// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 22, 26, 24, 28);

int cnt = 0;
void setup() {
  // myStepper.setSpeed(5); // For later
  // set the speed at 1 rpm:
  //myStepper.setSpeed(5);
  pinMode(leftBtn, INPUT_PULLUP);
  pinMode(rightBtn, INPUT_PULLUP);
  Serial.begin(9600);
  myStepper.setSpeed(60);
 
}

void loop() {
  // step one revolution  in one direction:
  
  // int sensorReading = analogRead(A0);
  // // map it to a range from 0 to 100:
  // int motorSpeed = map(sensorReading, 0, 1023, 0, 255);
  // // set the motor speed:
  // if (motorSpeed > 0) {
  //   myStepper.setSpeed(motorSpeed);
  // }


  // if(!digitalRead(leftBtn) && cnt < 1024){
  //   //Serial.println("2: "+digitalRead(2));
  //    myStepper.step(1);
  //    cnt ++;
  //    delay(10);
  // }
  
  // if(!digitalRead(rightBtn) && cnt > -1024){
  //     // step one revolution in the other direction:
  //   //Serial.println("3: "+digitalRead(3));
  //   myStepper.step(-1);
  //   cnt --;
  //   delay(10);
  // }
  myStepper.step(1);
  delay(10);


}
