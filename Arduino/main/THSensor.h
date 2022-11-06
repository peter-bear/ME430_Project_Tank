#ifndef THSENSOR_H
#define THSENSOR_H

#include <Arduino.h>
#include "Utilities.h"

#include "DHT.h"

#define DHTPIN 3
#define DHTTYPE DHT11


namespace me430
{

    class THSensor
    {
    public:
        THSensor();
        ~THSensor();
        
        void init();
        void update();
        float getTemperature();
        float getHumidity(); 

    private:
        DHT* dht;
        long lastTime;
       
    };

}

#endif