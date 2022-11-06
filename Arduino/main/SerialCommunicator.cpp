#include "SerialCommunicator.h"

using namespace me430;

SerialCommunicator::SerialCommunicator()
{
    input = "";
    onEventParsed = nullptr;
}

SerialCommunicator::~SerialCommunicator()
{
}

void SerialCommunicator::init(void (*func)(Event *e))
{
    Serial.begin(115200);
    Serial.println("Ready");
    this->onEventParsed = func;
}

void SerialCommunicator::onSerialEvent()
{
    while (Serial.available())
    {
        char inChar = (char)Serial.read();
        if (inChar == '\n')
        {
            Event e;
            parseEvent(&e);
            onEventParsed(&e);

            input = "";
        }
        else
        {
            input += inChar;
        }
    }
}

void SerialCommunicator::parseEvent(Event *e)
{
    if (input[0] == 'M')
    {
        int firstSapce = -1;
        int secondSapce = -1;

        for (int i = 2; i < input.length(); i++)
        {
            if (input[i] == ' ')
            {
                if (firstSapce == -1)
                {
                    firstSapce = i;
                }
                else
                {
                    secondSapce = i;
                }
            }
        }

        int x = input.substring(2, firstSapce).toInt();
        int y = input.substring(firstSapce + 1, secondSapce).toInt();
        int speed = input.substring(secondSapce + 1).toInt();

        e->type = me430::EventType::move;
        e->para1 = (byte)x;
        e->para2 = (byte)y;
        e->para3 = (byte)speed;
    }

    if (input[0] == 'G')
    {
        int sapce = -1;

        for (int i = 2; i < input.length(); i++)
        {
            if (input[i] == ' ')
            {
                sapce = i;
            }
        }

        int x = input.substring(2, sapce).toInt();
        int y = input.substring(sapce + 1).toInt();

        e->type = me430::EventType::gun;
        e->para1 = (byte)x;
        e->para2 = (byte)y;
    }

    if (input[0] == 'H')
    {
        e->type = me430::EventType::horn;
    }

    if (input[0] == 'F')
    {
        e->type = me430::EventType::fire;
    }
}