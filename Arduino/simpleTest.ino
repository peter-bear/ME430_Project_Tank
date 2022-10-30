/*
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

    Gun Desk Stepper
    PIN 22 - IN1
    PIN 24 - IN2
    PIN 26 - IN3
    PIN 28 - IN4

    Gun Move Stepper-Left
    PIN 30 - IN1
    PIN 32 - IN2
    PIN 34 - IN3
    PIN 36 - IN4

    Gun Move Stepper-right
    PIN 23 - IN1
    PIN 25 - IN2
    PIN 27 - IN3
    PIN 29 - IN4

    Gun shoot
    PIN 13 - Left Gun
    PIN 12 - Right Gun

*/
#include <Stepper.h>
#include <Servo.h>
#include "DHT.h"  

#define soundTrig 10
#define soundEcho 9

#define HLEnable 6
#define HREnable 5
#define HInput1 8
#define HInput2 7
#define HInput3 4
#define HInput4 2

#define GunL 13
#define GunR 12

#define DHTPIN 3     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

int serialLen = 7;
byte serialBuf[7];
bool serialComplete = false;

const byte endSignal = 255;
const int stepsPerRevolution = 2048; 
Stepper gunDeskStepper(stepsPerRevolution, 22, 26, 24, 28);
Stepper gunStepperL(stepsPerRevolution, 30, 34, 32, 36); 
Stepper gunStepperR(stepsPerRevolution, 23, 27, 25, 29);

int deskStepCounter = 0;
int gunStepCounter = 0;

int motorLSpeed, motorRSpeed;

DHT dht(DHTPIN, DHTTYPE);

int8_t temp,humi;
Servo myservo;

void setup(){
    Serial.begin(9600);
    Serial.println("Start");
    pinMode(HLEnable, OUTPUT);
    pinMode(HREnable, OUTPUT);
    pinMode(HInput1, OUTPUT);
    pinMode(HInput2, OUTPUT);
    pinMode(HInput3, OUTPUT);
    pinMode(HInput4, OUTPUT);
    pinMode(GunL, OUTPUT);
    pinMode(GunR, OUTPUT);
    pinMode(soundTrig, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(soundEcho, INPUT); // Sets the echoPin as an INPUT

    dht.begin();
    
    // init motor speed
    motorLSpeed = 0;
    motorRSpeed = 0;
    myservo.attach(13);

}

void loop(){
    
    myservo.write(0);
    analogWrite(HREnable, motorRSpeed);
    analogWrite(HLEnable, motorLSpeed);
    //getHumidityAndTemp();

    if(serialComplete){
        byte signal = serialBuf[0];
        if(signal == 0){

        }else if(signal == 1){
            int x = (serialBuf[1]<<8) | serialBuf[2];
            int y = (serialBuf[3]<<8) | serialBuf[4];
            int speed = (serialBuf[5]<<8) | serialBuf[6];
            //Serial.println(String("Motor Move --")+String("X: ")+x+String(" Y: ")+y+String("speed: ")+speed);
            //setMotorSpeed(x, y, speed);
        }else if(signal == 2){
            int x = (serialBuf[1]<<8) | serialBuf[2];
            int y = (serialBuf[3]<<8) | serialBuf[4];
            //Serial.println(String("Gun Desk --")+String("X: ")+x+String(" Y: ")+y);
            setGunDesk(x, y);
        }else if(signal == 3){
            gunShoot();
            //Serial.println("Shoot");
        }
        serialComplete = false;
    }
}

/*
    Set two motor's speed according to joystick's x and y position
*/
void setMotorSpeed(int x, int y, int speed){
    if(getUltrasonicDist() > 3){
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
        }
        else{
            motorLSpeed = 0;
            motorRSpeed = 0;
        }
    }
    else{
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

        digitalWrite(HInput1, HIGH);
        digitalWrite(HInput2, LOW);
        digitalWrite(HInput3, HIGH);
        digitalWrite(HInput4, LOW);  
          
    }else{
        digitalWrite(HInput1, LOW);
        digitalWrite(HInput2, HIGH);
        digitalWrite(HInput3, LOW);
        digitalWrite(HInput4, HIGH);  
    }
}

/*
    Set Tank Gun and Gun Desk Rotation
    x controls Gun Desk to turn left or right
    y controls Gun to turn up or down
    
    Gun Desk can only turn 180 degrees to the left or turn 180 degrees to the right
    Gun can only turn +- 30 degrees

*/
void setGunDesk(int x, int y){
    // gun desk rotate: turn left / right
    if(x > 0 && deskStepCounter > -stepsPerRevolution/2){
        gunDeskStepper.setSpeed(abs(x));
        gunDeskStepper.step(-1);
        deskStepCounter--;
        delay(10);
    }else if(x < 0 && deskStepCounter < stepsPerRevolution/2){
        gunDeskStepper.setSpeed(abs(x));
        gunDeskStepper.step(1);
        deskStepCounter++;
        delay(10);
    }

    // gun rotate: up or down
    if(y > 0 && gunStepCounter > -stepsPerRevolution/12){
        gunStepperL.setSpeed(abs(y));
        gunStepperR.setSpeed(abs(y));
        gunStepperL.step(-1);
        gunStepperR.step(-1);
        gunStepCounter--;
        delay(10);
    }else if(y < 0 && gunStepCounter < stepsPerRevolution/12){
        gunStepperL.setSpeed(abs(y));
        gunStepperR.setSpeed(abs(y));
        gunStepperL.step(1);
        gunStepperR.step(1);
        gunStepCounter++;
        delay(10);
    }    
}

/*
    Gun Fire
*/
void gunShoot(){
    digitalWrite(GunL, HIGH);
    digitalWrite(GunR, HIGH);
    delay(2000);
}


/**
 *  Sensor Parts
 */

// get temperature 
void getHumidityAndTemp(){
    byte signal = 4;
    humi = (int8_t)dht.readHumidity();
    temp = (int8_t)dht.readTemperature();
    long rst = (signal<<24) | (temp << 16) | (humi << 8) | endSignal;
    Serial.println(rst);
    delay(10000);
}

// get ultrasonic distance
int getUltrasonicDist(){
    digitalWrite(soundTrig, LOW);
    delay(2);
    digitalWrite(soundTrig, HIGH);
    delay(10);
    digitalWrite(soundTrig, LOW);

    return pulseIn(soundEcho, HIGH) * 0.034 / 2;
}



// read serial, stop serial listening after reading 255
void serialEvent(){
  int i = 0;
  while(Serial.available()&& !serialComplete){
  	byte tmp  = Serial.read();
    //Serial.println(tmp);
    if(tmp == endSignal){
      serialComplete = true;
    }
    serialBuf[i] = tmp;
    i++;
  }
  
}
