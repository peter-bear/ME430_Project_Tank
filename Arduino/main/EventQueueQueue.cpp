#include "EventQueue.h"

using namespace me430;

EventQueue::EventQueue()
{
    head = nullptr;
    tail = nullptr;
    size = 0;
}

EventQueue::~EventQueue()
{
}

bool EventQueue::push(Event *e)
{
    if (head == nullptr)
    {
        Node *n = new Node;
        memcpy(&n->e, e, sizeof(Event));
        n->next = nullptr;

        this->head = n;
        this->tail = n;

        this->size++;
        return true;
    }

    Node *n = new Node;
    memcpy(&n->e, e, sizeof(Event));
    n->next = nullptr;
    tail->next = n;
    this->tail = n;

    this->size++;
    return true;
}

Event EventQueue::pop()
{

    if (head == nullptr)
    {
        Event e;
        memset(&e, 0, sizeof(Event));
        return e;
    }

    Node *first = head;
    Event firstEvent = first->e;

    this->head = first->next;
    delete first;

    if (size == 1)
    {
        head = nullptr;
        tail = nullptr;
    }

    this->size--;
    return firstEvent;
}

bool EventQueue::isEmpty()
{
    return this->size == 0;
}

unsigned int EventQueue::getSize()
{
    return this->size;
}