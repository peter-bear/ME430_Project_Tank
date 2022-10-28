/*
Servo Motor
PIN 11


Ultra sound distance
PIN 10 - TRIG
PIN 9 - ECHO

H-bridge
PIN 6 - 1,2 Enable
PIN 5 - 3,4 Enable
PIN 8 - Input 1
PIN 7 - Input 2
PIN 4 - Input 3
PIN 2 - Input 4


*/

#include <Servo.h>
#define servoPin 11
#define ultraSoundT 10
#define ultraSoundE 8
#define HLEnable 6
#define HREnable 5
#define HInput1 8
#define HInput2 7
#define HInput3 4
#define HInput4 2

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

    // init motor speed
    motorLSpeed = 0;
    motorRSpeed = 0;

    //setMotorSpeed(0, 100, 200);

}

void loop(){
    
    analogWrite(HREnable, motorRSpeed);
    analogWrite(HLEnable, motorLSpeed);


    delay(10);
}

/*
Set two motor's speed according to joystick's x and y position
*/
void setMotorSpeed(int x, int y, int speed){
    if(x >= 0 && y > 0){
        motorLSpeed = speed;
        motorRSpeed = speed - x;
        setMotorDirection(true);
    }else if(x <= 0 && y > 0){
        motorLSpeed = speed + x;
        motorRSpeed = speed;
        setMotorDirection(true);
    }else if(x >= 0 && y < 0){
        motorLSpeed = speed;
        motorRSpeed = speed - x;
        setMotorDirection(false);
    }else if(x <= 0 && y < 0){
        motorLSpeed = speed + x;
        motorRSpeed = speed;
        setMotorDirection(false);
    }else{
        motorLSpeed = 0;
        motorRSpeed = 0;
    }
}

/*
Set motor direction
up == true ? forward : backward
*/
void setMotorDirection(bool up){
    if(up){
        digitalWrite(HInput1, LOW);
        digitalWrite(HInput2, HIGH);
        digitalWrite(HInput3, LOW);
        digitalWrite(HInput4, HIGH);    
    }else{
        digitalWrite(HInput1, HIGH);
        digitalWrite(HInput2, LOW);
        digitalWrite(HInput3, HIGH);
        digitalWrite(HInput4, LOW);    
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