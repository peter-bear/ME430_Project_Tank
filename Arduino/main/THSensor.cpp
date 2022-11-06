#include "THSensor.h"

using namespace me430;

THSensor::THSensor()
{
    lastTime = 0;
}

THSensor::~THSensor()
{

}

void THSensor::init()
{
    dht = new DHT(DHTPIN, DHTTYPE);
    dht->begin();
}

void THSensor::update()
{
}

float THSensor::getTemperature()
{
    return dht->readTemperature();
}

float THSensor::getHumidity()
{
    return dht->readHumidity();
}