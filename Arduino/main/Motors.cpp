#include "Motors.h"

using namespace me430;

Motors::Motors()
{
    pinMode(HLEnable, OUTPUT);
    pinMode(HREnable, OUTPUT);
    pinMode(HInput1, OUTPUT);
    pinMode(HInput2, OUTPUT);
    pinMode(HInput3, OUTPUT);
    pinMode(HInput4, OUTPUT);

    digitalWrite(HInput1, LOW);
    digitalWrite(HInput2, LOW);
    digitalWrite(HInput3, LOW);
    digitalWrite(HInput4, LOW);

    this->x = 50;
    this->y = 50;
    this->speed = 0;
}

Motors::~Motors()
{
}

void Motors::update() 
{
    ultrasound.update();
    if(ultrasound.getDistance() <= 5)
    {
        sync();
    }

}

void Motors::onEvent(Event *e)
{
    this->x = e->para1;
    this->y = e->para2;
    this->speed = e->para3;
}

void Motors::sync()
{
    if(ultrasound.getDistance() <= 5){
        digitalWrite(HInput1, LOW);
        digitalWrite(HInput2, LOW);
        digitalWrite(HInput3, LOW);
        digitalWrite(HInput4, LOW);
        return;
    }

    if (y > MOTOR_THREHOLD)
    {

        digitalWrite(HInput1, LOW);
        digitalWrite(HInput2, HIGH);
        digitalWrite(HInput3, LOW);
        digitalWrite(HInput4, HIGH);
    }
    else if (y < MOTOR_THREHOLD)
    {

        digitalWrite(HInput1, HIGH);
        digitalWrite(HInput2, LOW);
        digitalWrite(HInput3, HIGH);
        digitalWrite(HInput4, LOW);
    }
    else
    {
        digitalWrite(HInput1, LOW);
        digitalWrite(HInput2, LOW);
        digitalWrite(HInput3, LOW);
        digitalWrite(HInput4, LOW);
    }

    if (x > MOTOR_THREHOLD)
    {
        analogWrite(HLEnable, speed * 5);
        analogWrite(HREnable, (speed - (x - MOTOR_THREHOLD)) * 5);
    }
    else if (x < MOTOR_THREHOLD)
    {
        analogWrite(HLEnable, (MOTOR_THREHOLD - (speed - x)) * 5);
        analogWrite(HREnable, speed * 5);
    }
    else
    {
        analogWrite(HLEnable, speed * 5);
        analogWrite(HREnable, speed * 5);
    }
}
