#include "Guns.h"

using namespace me430;

Guns::Guns()
{
    speed = 10;

    stepperL = new Stepper(STEPPER_STEPS_PER_REVOLUTION, 30, 34, 32, 36);
    stepperL->setSpeed(speed);
    stepperR = new Stepper(STEPPER_STEPS_PER_REVOLUTION, 23, 27, 25, 29);
    stepperR->setSpeed(speed);

    x = 50;
    lastStepTime = 0;
    step = 0;
}

Guns::~Guns()
{
}

void Guns::onEvent(Event *e)
{
    if (e->para2 > 50)
    {
        step = 1;
    }
    else if (e->para2 < 50)
    {
        step = -1;
    }
    else
    {
        step = 0;
    }
}

void Guns::update()
{

    // long nextDisplacement = speed * (totalSteps + step);

    // if (nextDisplacement < -1000 || nextDisplacement > 600)
    //     return;

    if (millis() - lastStepTime > 1)
    {

        if (step != 0)
        {
            stepperL->step(step);
            stepperR->step(step*(-1));
            totalSteps += step;
        }

        lastStepTime = millis();
    }
}

void Guns::sync()
{
}