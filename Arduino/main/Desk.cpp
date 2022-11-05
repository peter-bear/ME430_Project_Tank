#include "Desk.h"

using namespace me430;

Desk::Desk()
{
    stepper = new Stepper(STEPPER_STEPS_PER_REVOLUTION, 22, 26, 24, 28);
    stepper->setSpeed(10);

    x = 50;
    lastStepTime = 0;
    step = 0;
}

Desk::~Desk()
{
}

void Desk::onEvent(Event *e)
{
    if (e->para1 > 50)
    {
        step = 1;
    }
    else if (e->para1 < 50)
    {
        step = -1;
    }
    else
    {
        step = 0;
    }
}

void Desk::update()
{
    if (millis() - lastStepTime > 1)
    {
        if (step != 0)
        {
            stepper->step(step);
        }

        lastStepTime = millis();
    }
}

void Desk::sync()
{
}