#include "Utilities.h"

#ifndef EVENT_QUEUE
#define EVENT_QUEUE

namespace me430
{
    struct Node
    {
        Event e;
        Node *next;
    };

    class EventQueue
    {

    public:
        EventQueue();
        ~EventQueue();

        bool isEmpty();
        unsigned int getSize();

        bool push(Event *e);
        Event pop();

    private:
        Node *head;
        Node *tail;

        unsigned int size;
    };

}

#endif