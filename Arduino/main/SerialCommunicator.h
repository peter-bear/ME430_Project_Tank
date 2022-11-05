#ifndef SERIAL_COMMUNICATOR_H
#define SERIAL_COMMUNICATOR_H

#include <Arduino.h>
#include "Utilities.h"

namespace me430
{

    class SerialCommunicator
    {
    public:
        SerialCommunicator();
        ~SerialCommunicator();

        void init(void (*func)(Event *e));
        void onSerialEvent();

    private:
        void parseEvent(Event *e);

    private:
        String input;
        void (*onEventParsed)(Event *e);
    };

}

#endif