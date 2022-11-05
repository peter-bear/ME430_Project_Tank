#ifndef GUNS_H
#define GUNS_H

#include <Stepper.h>
#include "Utilities.h"

namespace me430
{
    class Guns
    {
    public:
        Guns();
        ~Guns();

        void onEvent(Event *e);
        void update();
        void sync();

    private:
        Stepper *stepperL;
        Stepper *stepperR;
        byte x;
        long speed;
        int8_t step;
        long totalSteps;

        unsigned long lastStepTime;
    };
}

#endif