#ifndef MOTORS_H
#define MOTORS_H

#include "Utilities.h"

#define MOTOR_THREHOLD 50

#define HLEnable 6
#define HREnable 5

#define HInput1 8
#define HInput2 7
#define HInput3 4
#define HInput4 2

namespace me430
{
    class Motors
    {
    public:
        Motors();
        ~Motors();

        void onEvent(Event *e);
        void update();
        void sync();

    private:
        byte x;
        byte y;
        byte speed;
    };
}

#endif