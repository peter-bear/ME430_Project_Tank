#include "Horn.h"

using namespace me430;

Horn::Horn()
{
    isHorning = false;
}

Horn::~Horn()
{

}

void Horn::onEvent(Event *e)
{
    

    if(isHorning){
        noTone(HORN_PIN);   
    }
    else 
    {
        tone(HORN_PIN, 600);
    }

    isHorning = !isHorning;
}