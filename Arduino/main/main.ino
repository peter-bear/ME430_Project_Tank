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

#include <ArduinoSTL.h>
#include <Stepper.h>
#include <Servo.h>

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

#define DHTPIN 3      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

#define MOTOR_THREHOLD 50
#define STEPPER_THREHOLD 50

const byte endSignal = 255;
const int stepsPerRevolution = 2048;

Stepper gunDeskStepper(stepsPerRevolution, 22, 26, 24, 28);
Stepper gunStepperL(stepsPerRevolution, 30, 34, 32, 36);
Stepper gunStepperR(stepsPerRevolution, 23, 27, 25, 29);

Servo myServo;

#define EVENT_UNKNOWN 0
#define EVENT_M 1
#define EVENT_G 2

struct Event
{
  byte type;
  byte para1;
  byte para2;
  byte para3;
};

std::vector<Event> eventList;

String g_serialInput;
byte motorX;
byte motorY;
byte motorSpeed;

byte deskStepperX;
byte gunServoY;

int deskStepperStep;
int gunStepperStep;
unsigned long deskStepperLastTime;

void setup()
{
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
  pinMode(soundEcho, INPUT);  // Sets the echoPin as an INPUT

  motorX = MOTOR_THREHOLD;
  motorY = MOTOR_THREHOLD;
  motorSpeed = MOTOR_THREHOLD;

  digitalWrite(HInput1, LOW);
  digitalWrite(HInput2, LOW);
  digitalWrite(HInput3, LOW);
  digitalWrite(HInput4, LOW);

  myServo.attach(13);
  gunDeskStepper.setSpeed(10);
  gunStepperL.setSpeed(5);
  gunStepperR.setSpeed(5);
  
  deskStepperStep = 0;
  gunStepperStep = 0;
  gunServoY = 5;

  deskStepperLastTime = millis();
  

}

void loop()
{


  digitalWrite(HREnable, HIGH);
  digitalWrite(HLEnable, HIGH);

  if(eventList.size() != 0){
    Event e = eventList.front();
    eventList.erase(eventList.begin());

    switch (e.type)
    {
    case EVENT_M:
      Serial.println("GET M");
      motorX = e.para1;
      motorY = e.para2;
      motorSpeed = e.para3;
      break;

    case EVENT_G:
      Serial.println("GET G");

      if(e.para1 > 50){
        deskStepperStep = 1;
      } else if(e.para1 < 50){
        deskStepperStep = -1;
      } else {
        deskStepperStep = 0;
      }

      gunServoY = 10 - e.para2/10;
      
      
      //e.para2;
      break;
    
    default:
      break;
    }

  }
  
  syncState();

}

void syncState()
{
  if (motorY > MOTOR_THREHOLD) {

       digitalWrite(HInput1, LOW);
       digitalWrite(HInput2, HIGH);
       digitalWrite(HInput3, LOW);
       digitalWrite(HInput4, HIGH);

       
  } else if (motorY < MOTOR_THREHOLD) {

       digitalWrite(HInput1, HIGH);
       digitalWrite(HInput2, LOW);
       digitalWrite(HInput3, HIGH);
       digitalWrite(HInput4, LOW);
       
  } else {
      digitalWrite(HInput1, LOW);
      digitalWrite(HInput2, LOW);
      digitalWrite(HInput3, LOW);
      digitalWrite(HInput4, LOW);
  }


    if( millis() - deskStepperLastTime > 1){
      if(deskStepperStep != 0){
        gunDeskStepper.step(deskStepperStep);
      }
      deskStepperLastTime = millis();
    }

    if(gunServoY != 5){
      myServo.write(gunServoY);
    }
    

}


void parseEvent(String input)
{

  Event e;

  if(input[0] == 'M'){
    int firstSapce = -1;
      int secondSapce = -1;

      for(int i=2;i<input.length();i++){
        if (input[i] == ' '){
          if(firstSapce == -1){
            firstSapce = i;
          } else {
            secondSapce = i;
          }
        }
      }

      int x = input.substring(2, firstSapce).toInt();
      int y = input.substring(firstSapce + 1, secondSapce).toInt();
      int speed = input.substring(secondSapce + 1).toInt();

      
      e.type = EVENT_M;
      e.para1 = (byte)x;
      e.para2 = (byte)y;
      e.para3 = (byte)speed;

      eventList.push_back(e);
  }

  if(input[0] == 'G'){
    int sapce = -1;

      for(int i=2;i<input.length();i++){
        if (input[i] == ' '){
          sapce = i;
        }
      }

      int x = input.substring(2, sapce).toInt();
      int y = input.substring(sapce + 1).toInt();
      
      e.type = EVENT_G;
      e.para1 = (byte)x;
      e.para2 = (byte)y;

      eventList.push_back(e);
  }

  return;

  
}

// read serial, stop serial listening after reading 255
void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '\n')
    {
      Serial.println(g_serialInput);
      parseEvent(g_serialInput);
      g_serialInput = "";
    }
    else
    {
      g_serialInput += inChar;
    }
  }
}