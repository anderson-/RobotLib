/**
 * @file IRProximitySensor.cpp
 * @author Diego Lee <diegolee7@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 by Fernando Ferreira <fpf.padilha@gmail.com>
 * 
 * RobotLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RobotLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with RobotLib.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "IRProximitySensor.h"

IRProximitySensor::IRProximitySensor(uint8_t pin) : Device(false, true),
                                  pin(pin){}

void IRProximitySensor::begin(){}

void IRProximitySensor::stop(){}

void IRProximitySensor::reset(){}

void IRProximitySensor::update(){
    int leitura = analogRead(pin);
    float tensao = (float)leitura*(5.0 / 1024.0);
    distance = (int)(16.2537*tensao*tensao*tensao*tensao - 129.893*tensao*tensao*tensao
                    + 382.268*tensao*tensao- 512.611*tensao + 306.439);
}

bool IRProximitySensor::isReady(){
  return true;
}

uint8_t IRProximitySensor::get(uint8_t * buffer, uint8_t size){

    uint8_t *temp = (uint8_t*)&distance;
    uint8_t i;
    for(i = 0; i<sizeof(distance);i++)
    {
        buffer[i] = temp[i];
    }
    return 2;

}

int IRProximitySensor::getDistance(){
	return distance;
}

void IRProximitySensor::set(const uint8_t * data, uint8_t size){

}
