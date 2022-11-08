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

#include "Utilities.h"
#include "EventQueue.h"
#include "Motors.h"
#include "SerialCommunicator.h"
#include "Desk.h"
#include "Guns.h"
#include "Screens.h"
#include "THSensor.h"
#include "Horn.h"
#include "Fire.h"

me430::SerialCommunicator g_serialCommunicator;
me430::EventQueue g_eventQueue;
me430::Motors g_motors;
me430::Desk g_desk;
me430::Guns g_guns;
me430::Screens g_screens;
me430::Horn g_horn;
me430::Fire g_fire;

void onEventParsed(me430::Event *e);

void setup()
{
  g_serialCommunicator.init(&onEventParsed);
  g_screens.init();
}

void loop()
{

  if (!g_eventQueue.isEmpty())
  {
    me430::Event e = g_eventQueue.pop();

    switch (e.type)
    {
    case me430::EventType::move:
      g_motors.onEvent(&e);
      g_motors.sync();
      break;

    case me430::EventType::gun:
      g_desk.onEvent(&e);
      g_guns.onEvent(&e);
      break;

    case me430::EventType::horn:
      g_horn.onEvent(&e);
      break;

    case me430::EventType::fire:
      g_fire.onEvent(&e);
      break;

    default:
      Serial.println(String("Event TYPE") + e.type);
      break;
    }
  }

  update();
}

void update()
{
  g_motors.update();
  
  g_desk.update();
  g_guns.update();
  g_screens.update();

}

void onEventParsed(me430::Event *e)
{
  g_eventQueue.push(e);
}

void serialEvent()
{
  g_serialCommunicator.onSerialEvent();
}
