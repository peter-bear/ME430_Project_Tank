/*
Servo Motor
PIN 11


Ultra sound distance
PIN 10 - TRIG
PIN 9 - ECHO

H-bridge
PIN 13 - 1,2 Enable
PIN 12 - 3,4 Enable
PIN 8 - Input 1
PIN 7 - Input 2
PIN 6 - Input 3
PIN 5 - Input 4


*/

#include <Servo.h>
#define servoPin 11
#define ultraSoundT 10
#define ultraSoundE 8
#define HLEnable 13
#define HREnable 12
#define HInput1 8
#define HInput2 7
#define HInput3 6
#define HInput4 5

String inputString ="";
bool stringComplete = false;
Servo shootGun;

uint16_t motorLSpeed;
uint16_t motorRSpeed;

void setup(){
    Serial.begin(9600);

    pinMode(HLEnable, OUTPUT);
    pinMode(HREnable, OUTPUT);
    pinMode(HInput1, OUTPUT);
    pinMode(HInput2, OUTPUT);
    pinMode(HInput3, OUTPUT);
    pinMode(HInput4, OUTPUT);
    
    shootGun.attach(servoPin);

    motorLSpeed = 0;
    motorRSpeed = 0;
}

void loop(){
    
    // 摇杆向前， 小车向前移动，2 wheel 速度与位移y有关
    digitalWrite(HREnable, motorLSpeed);
    digitalWrite(HLEnable, motorRSpeed);


    delay(10);
}

void setMotorSpeed(uint16_t x, uint16_t y){
    if(x > 0){
        motorLSpeed += x;
    }else{
        motorLSpeed = 0;
        motorRSpeed = 0;
    }
}

// // read serial
// void serialEvent(){
//   while(Serial.available()){
//   	char inChar = (char)Serial.read();
//     if (inChar == '.'){
//     	stringComplete = true;
//     }else{
//     	inputString += inChar;
//     }
//   }
// }