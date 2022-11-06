#ifndef HORN_H
#define HORN_H

#include <Arduino.h>
#include "Utilities.h"

#define HORN_PIN 11

namespace me430
{
    class Horn
    {
    public:
        Horn();
        ~Horn();

        void onEvent(Event *e);
        void update();
        void sync();

    private:
        bool isHorning;
    };
}

#endif