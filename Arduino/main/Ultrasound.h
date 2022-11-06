#ifndef ULTRASOUND_H
#define ULTRASOUND_H

#include <Arduino.h>
#include "Utilities.h"

#define echoPin 9 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 10 //attach pin D3 Arduino to pin Trig of HC-SR04

namespace me430
{

    class Ultrasound
    {
    public:
        Ultrasound();
        ~Ultrasound();
        
        void update();
        int getDistance();    

    private:
        long duration; // variable for the duration of sound wave travel
        int distance; // variable for the distance measurement
       
    };

}

#endif