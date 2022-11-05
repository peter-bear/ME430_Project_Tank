
#include <Arduino.h>

#ifndef UTILITIES_H
#define UTILITIES_H

namespace me430
{
  struct Event
  {
    byte type;
    byte para1;
    byte para2;
    byte para3;
  };

  enum EventType
  {
    empty,
    unknown,
    move,
    gun
  };
}

#define STEPPER_STEPS_PER_REVOLUTION 2048

#endif