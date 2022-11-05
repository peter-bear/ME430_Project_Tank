#ifndef DESK_H
#define DESK_H

#include <Stepper.h>
#include "Utilities.h"

namespace me430
{
    class Desk
    {
    public:
        Desk();
        ~Desk();

        void onEvent(Event *e);
        void update();
        void sync();

    private:
        Stepper *stepper;
        byte x;
        unsigned long lastStepTime;
        int8_t step;
    };
}

#endif