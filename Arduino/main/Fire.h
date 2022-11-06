#ifndef FIRE_H
#define FIRE_H

#include <Arduino.h>
#include "Utilities.h"

#define FIRE_PIN 12

namespace me430
{
    class Fire
    {
    public:
        Fire();
        ~Fire();

        void onEvent(Event *e);
        void update();
        void sync();

    private:
        bool isFiring;
    };
}

#endif