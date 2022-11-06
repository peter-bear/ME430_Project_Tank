#include "Fire.h"

using namespace me430;

Fire::Fire()
{
    pinMode(FIRE_PIN, OUTPUT);
    isFiring = false;
}

Fire::~Fire()
{

}

void Fire::onEvent(Event *e)
{

    if(isFiring){
        digitalWrite(FIRE_PIN, LOW);
    }
    else 
    {
        digitalWrite(FIRE_PIN, HIGH);
    }

    isFiring = !isFiring;
}