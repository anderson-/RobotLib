/**
 * @file IRProximitySensor.cpp
 * @author Fernando Ferreira <fpf.padilha@gmail.com>
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

#include "ReflectanceSensorArray.h"
#include "Debug.h"

ReflectanceSensorArray::ReflectanceSensorArray(uint8_t pin_out, const uint8_t pins_sel[], uint16_t thld) : Device(false, true, 4),
            pin_out(pin_out),
            thld(thld),
            value(0) {
  uint8_t i;
  this->pins_sel[0] = pins_sel[0];
  this->pins_sel[1] = pins_sel[1];
  this->pins_sel[2] = pins_sel[2];
  analog = 1;
}

ReflectanceSensorArray::ReflectanceSensorArray(const uint8_t pins[]) : Device(false, true, 4),
            value(0) {
  uint8_t i;
  for (i = 0; i < NUM_SENSORS; i++) {
    this->pins[i] = pins[i];
  }
  analog = 0;
}

void ReflectanceSensorArray::begin() {
  uint8_t i;
  if (analog) {
	  pinMode(pin_out, INPUT);
	  pinMode(pins_sel[0], OUTPUT);
	  pinMode(pins_sel[1], OUTPUT);
	  pinMode(pins_sel[2], OUTPUT);
  } else {
	  for (i = 0; i < NUM_SENSORS; i++) {
		pinMode(pins[i],INPUT);
	  }
  }
}

void ReflectanceSensorArray::stop() {
  
}

void ReflectanceSensorArray::reset() {
  value = 0;
  begin();
}

void ReflectanceSensorArray::update(){
  uint8_t i;
  value = 0;
  for (i = 0; i < NUM_SENSORS; i++) {
	if (analog) {
		digitalWrite(pins_sel[0], (i & 1));
		digitalWrite(pins_sel[1], ((i >> 1) & 1));
		digitalWrite(pins_sel[2], ((i >> 2) & 1));
		delayMicroseconds(100);
		value |= ((analogRead(pin_out) > thld) << i);
	} else {
		value |= (digitalRead(pins[i]) << i);
	}
  }
}

bool ReflectanceSensorArray::isReady() {
  return true;
}

uint8_t ReflectanceSensorArray::get(uint8_t * buffer, uint8_t size) {
  if(size > 0) {
    buffer[0] = value;
    return 1;
  }
}

void ReflectanceSensorArray::set(const uint8_t * data, uint8_t size) {
  if (size == 2) {
    //thld = (data[1] << 8) | data[0];
    thld = *((uint16_t *) data);
  }
}
