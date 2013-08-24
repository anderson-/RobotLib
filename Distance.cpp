/*
 * File:   Compass.cpp
 * Author: Anderson Antunes
 *
 * Created on July 18, 2013, 3:47 PM
 */

#include "Distance.h"

Distance::Distance(uint8_t pin) : Device(false, true),
                                  pin(pin){}

void Distance::begin(){}

void Distance::stop(){}

void Distance::reset(){}

void Distance::update(){
    int leitura = analogRead(pin);
    float tensao = (float)leitura*(5.0 / 1024.0);
    distance = (int)(16.2537*tensao*tensao*tensao*tensao - 129.893*tensao*tensao*tensao
                    + 382.268*tensao*tensao- 512.611*tensao + 306.439);
}

bool Distance::isReady(){
  return true;
}

uint8_t Distance::get(uint8_t * buffer, uint8_t size){

    uint8_t *temp = (uint8_t*)&distance;
    uint8_t i;
    for(i = 0; i<sizeof(distance);i++)
    {
        buffer[i] = temp[i];
    }
    return 2;

}

void Distance::set(const uint8_t * data, uint8_t size){

}
